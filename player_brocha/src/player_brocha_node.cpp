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
        //Transform t1; //memoria local do jogo (cada um dos jogos individuais)//é necessário actualizar sempre que for chamada
        TransformListener listener;
        
        MyPlayer(string argin_name, string argin_team_name) : Player(argin_name, argin_team_name) {
            
            
            sub = n.subscribe("/make_a_play/cheetah", 1000, &MyPlayer::makeAPlayCallback, this);
            cout << "Initialized MyPlayer" << endl;
            
            Transform t1;
            
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

        float getAngleTo(string player_name, float time_to_wait = 0.1) {

            StampedTransform trans;
            ros::Time now = Time(0);

            try {
                listener.waitForTransform(name, player_name, now, Duration(time_to_wait));
                listener.lookupTransform(name, player_name, now, trans);

            } catch (TransformException &ex) {

                ROS_ERROR("%s", ex.what());
                Duration(0.01).sleep();
            }

            float x = trans.getOrigin().x();
            float y = trans.getOrigin().y();

            return atan2(y, x);
        }
        
        tf::StampedTransform getPose( float time_to_wait = 0.1){
            //função vai atualizar o sistema e vai meter todos em sincronizmo
            //Transform t1;
            StampedTransform trans;
            Time now = Time(0);    
            try {
                listener.waitForTransform("map", name, now, Duration(time_to_wait));
                listener.lookupTransform("map",name , now, trans);

            } catch (TransformException &ex) {

                ROS_ERROR("%s", ex.what());
                Duration(0.01).sleep();
            }
                                               
            return trans;
            
        }
        
        void move(float displacement,float max_displacement, float turn_angle,float max_turn_angle){
      
            //float displacement = msg->max_displacement;
                        
            //max turn angle
            if (turn_angle > max_turn_angle) turn_angle = max_turn_angle;
            else if (turn_angle < -max_turn_angle) turn_angle = -max_turn_angle;
            //max displacmente
            if (displacement > max_displacement) displacement = max_displacement;
            else if (displacement < -max_displacement) displacement = -max_displacement;
            
            
            //compute the new reference frame
            Transform t_mov;
            Quaternion q;
            q.setRPY(0, 0, turn_angle);

            t_mov.setRotation(q);
            t_mov.setOrigin(Vector3(displacement, 0.0, 0.0));
            
            Transform t = getPose() * t_mov;

            //Send the new transform to ROS

            br.sendTransform(StampedTransform(t, Time::now(), "/map", name));
            
       
        }
        /*
        string getNear(void){
            
            StampedTransform trans1;
            StampedTransform trans2;
            StampedTransform trans3;
            
            ros::Time now = Time(0);
            
            try {
                
                listener.waitForTransform(name, "dcorreira", now, Duration(time_to_wait));
                listener.lookupTransform(name, "dcorreira", now, trans1);
                
                listener.waitForTransform(name, "vsilva", now, Duration(time_to_wait));
                listener.lookupTransform(name, "vsilva", now, trans2);
                
                listener.waitForTransform(name, "jsousa", now, Duration(time_to_wait));
                listener.lookupTransform(name, "jsousa", now, trans3);
                
                
            } catch (TransformException &ex) {

                ROS_ERROR("%s", ex.what());
                Duration(0.01).sleep();
            }
                       
            float d1,d2,d3;
            
            float x1 = trans1.getOrigin().x();
            float y1 = trans1.getOrigin().y();
            d1=atan2(y1, x1);
            
            float x2 = trans2.getOrigin().x();
            float y2 = trans2.getOrigin().y();
            d2=atan2(y2, x2);
            
            float x3 = trans3.getOrigin().x();
            float y3 = trans3.getOrigin().y();
            d3=atan2(y3, x3);
            
            if(d1>d2){return name="dcorreira";}
            
            
            
            return atan2(y, x);
            
            
        }*/
        
        
        void makeAPlayCallback(const rwsua2017_msgs::MakeAPlay::ConstPtr& msg) {
            
                  //send a information basic to move player
            //getNearbyPlayer();
            
            float turn_angle = getAngleTo("fsilva");
            float displacement =msg->max_displacement;
            //float max_turn_angle = (M_PI / 30);
                        
            cout << "msg: max displacement -> " << msg->max_displacement << endl;
            
            move(displacement, msg->max_displacement, turn_angle, M_PI/30);      

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

