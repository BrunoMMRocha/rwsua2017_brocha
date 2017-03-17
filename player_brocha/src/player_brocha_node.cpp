#include <iostream>  
#include <vector>    
//ROS INCLUDES
#include <ros/ros.h>  
#include <rwsua2017_libs/player.h>   
#include <rwsua2017_msgs/MakeAPlay.h>                                                                            
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>

using namespace std;
//using namespace boost;
using namespace tf;
using namespace ros;

namespace rwsua2017 {

    class MyPlayer : public Player //inherits the class Player
    {
    public:

        //properties
        Subscriber sub;
        TransformBroadcaster br;
        Transform t1;
        TransformListener listener;

        MyPlayer(string argin_name, string argin_team_name) : Player(argin_name, argin_team_name) {


            sub = n.subscribe("/make_a_play/cheetah", 1000, &MyPlayer::makeAPlayCallback, this);
            cout << "Initialized MyPlayer" << endl;

            t1.setOrigin(Vector3(randNumber(), randNumber(), 0.0));
            Quaternion q;
            q.setRPY(0, 0, 0);
            t1.setRotation(q);
            br.sendTransform(StampedTransform(t1, Time::now(), "map", name));
        };

        double randNumber() {
            struct timeval t1;
            gettimeofday(&t1, NULL);
            srand(t1.tv_usec);
            double x = ((((double) rand() / (double) RAND_MAX)*2 - 1)*5);

            return x;
        }

        float getAngleTo(string player_name) {

            StampedTransform trans;

            try {
                listener.lookupTransform(name, player_name, Time(0), trans);

            } catch (TransformException &ex) {

                ROS_ERROR("%s", ex.what());
                Duration(0.01).sleep();
            }4

            float x = trans.getOrigin().x();
            float y = trans.getOrigin().y();

            return atan2(y, x);
        }

        void makeAPlayCallback(const rwsua2017_msgs::MakeAPlay::ConstPtr& msg) {


            cout << "msg: max displacement -> " << msg->max_displacement << endl;
            //send a information basic to move player
            float turn_angle = getAngleTo("bvieira");
            float displacement = msg->max_displacement;

            double max_t = (M_PI / 30);

            if (turn_angle > max_t) turn_angle = max_t;
            else if (turn_angle < -max_t) turn_angle = -max_t;

            //compute the new reference frame
            Transform t_mov;
            Quaternion q;
            q.setRPY(0, 0, turn_angle);

            t_mov.setRotation(q);
            t_mov.setOrigin(Vector3(displacement, 0.0, 0.0));

            Transform t = t1 * t_mov;

            //Send the new transform to ROS

            br.sendTransform(StampedTransform(t, Time::now(), "/map", name));
            t1 = t;

        }




        vector<string> teammates;
    };
}

int main(int argc, char **argv) {
    //because we used <using namespace std>  we can replace the other line
    //std::cout << "Hello world" << std::endl;
    cout << "Hello world" << endl;

    init(argc, argv, "player_brocha");

    //Creating an instance of class Player
    rwsua2017::MyPlayer myplayer("brocha", "red");

    //cout << "Created an instance of class player with public name " << player.name << endl;
    cout << "name = " << myplayer.name << endl;

    // size_t = unsigned long int (sortchut)
    for (size_t i = 0; i < myplayer.teammates.size(); ++i) {

        cout << myplayer.teammates[i] << endl;
    }

    spin();

    return 1;
}



//std::vector <boost::shared_pts<Player>> teammates;

