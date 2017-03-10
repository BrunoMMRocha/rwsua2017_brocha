#include <iostream>  
#include <vector>    
//ROS INCLUDES
#include <ros/ros.h>  
#include <rwsua2017_libs/player.h>   
#include <rwsua2017_msgs/MakeAPlay.h>                                                                            

using namespace std;

namespace rwsua2017 {

       
    class MyPlayer : public Player //inherits the class Player
    {
    public:
	
        
        ros::Subscriber sub;
               
		
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
			
            sub = n.subscribe("/make_a_play", 1000, &MyPlayer::makeAPlayCallback, this);
            cout << "Initialized MyPlayer" << endl;
        }

        void makeAPlayCallback(const rwsua2017_msgs::MakeAPlay::ConstPtr& msg) {
            cout << "msg: max displacement -> " << msg->max_displacement << endl;
        }


        vector<string> teammates;
    };
}

int main(int argc, char **argv) {
    //because we used <using namespace std>  we can replace the other line
    //std::cout << "Hello world" << std::endl;
    cout << "Hello world" << endl;

    ros::init(argc, argv, "player_brocha");

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

    ros::spin();

    return 1;
}



//std::vector <boost::shared_pts<Player>> teammates;

