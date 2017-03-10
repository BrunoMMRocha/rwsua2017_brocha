#include <iostream>  
#include <vector>    
//ROS INCLUDES
#include <ros/ros.h>  
#include <rwsua2017_libs/player.h>   
#include <rwsua2017_msgs/MakeAPlay.h>                                                                            
#include <tf/transform_broadcaster.h>

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
		
        MyPlayer(string argin_name, string argin_team_name) : Player(argin_name, argin_team_name) {

           /*
            n.getParam("red", red_team);
            n.getParam("green", green_team);
            n.getParam("blue", blue_team);
			
			cout << "red_team:"<<endl;
            for (size_t i=0;i<red_team.size();i++)
                cout << red_team[i]<<endl;
            
            cout << "green_team:"<<endl;
            for (size_t i=0;i<green_team.size();i++)
                cout << green_team[i]<<endl;
            
            cout << "blue_team:"<<endl;
            for (size_t i=0;i<blue_team.size();i++)
                cout << blue_team[i]<<endl;
			*/
			
            sub = n.subscribe("/make_a_play/cheetah", 1000, &MyPlayer::makeAPlayCallback, this);
            cout << "Initialized MyPlayer" << endl;
			
			t1.setOrigin( Vector3(1,1, 0) );
            Quaternion q;
			q.setRPY(0, 0, 0);
			t1.setRotation(q);
			br.sendTransform(StampedTransform(t1, Time::now(), "map", name));
			
        
        
        }

        void makeAPlayCallback(const rwsua2017_msgs::MakeAPlay::ConstPtr& msg) {
			
								
            cout << "msg: max displacement -> " << msg->max_displacement << endl;
            //send a information basic to move player
            float turn_angle=M_PI/10;
            float displacement = 0.5;
            
            //compute the new reference frame
            Transform t_mov;
            Quaternion q;
            q.setRPY(0, 0, turn_angle);
            
            t_mov.setRotation(q);
            t_mov.setOrigin( Vector3(displacement,0.0, 0.0) );
            
			Transform t=t1 * t_mov;
			
			//Send the new transform to ROS
					
			br.sendTransform(StampedTransform(t, Time::now(), "/map", name));
			
			t1=t;
			
			//caracteristics to  move the player
			
			         
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
    cout << "team name = " << myplayer.get_team_name() << endl;

    myplayer.teammates.push_back("rodolfo");
    myplayer.teammates.push_back("arnaldo");



    // size_t = unsigned long int (sortchut)
    for (size_t i = 0; i < myplayer.teammates.size(); ++i) {
        cout << myplayer.teammates[i] << endl;
    }

    spin();

    return 1;
}



//std::vector <boost::shared_pts<Player>> teammates;

