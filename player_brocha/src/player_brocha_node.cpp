#include <iostream>  
#include <vector>    
//ROS INCLUDES
#include <ros/ros.h>  
#include <rwsua2017_libs/player.h>   
#include <rwsua2017_msgs/MakeAPlay.h>                                                                            
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>
#include <visualization_msgs/Marker.h>

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
        ros::Publisher vis_pub;

        MyPlayer(string argin_name, string argin_team_name) : Player(argin_name, argin_team_name) {


            sub = n.subscribe("/make_a_play/cheetah", 1000, &MyPlayer::makeAPlayCallback, this);
            cout << "Initialized MyPlayer" << endl;
            vis_pub = n.advertise<visualization_msgs::Marker>("/bocas", 0);

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

        tf::StampedTransform getPose(float time_to_wait = 0.1) {
            //função vai atualizar o sistema e vai meter todos em sincronizmo
            //Transform t1;
            StampedTransform trans;
            Time now = Time(0);
            try {
                listener.waitForTransform("map", name, now, Duration(time_to_wait));
                listener.lookupTransform("map", name, now, trans);

            } catch (TransformException &ex) {

                ROS_ERROR("%s", ex.what());
                Duration(0.01).sleep();
            }

            return trans;

        }

        void move(float displacement, float max_displacement, float turn_angle, float max_turn_angle) {

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

            //Transform t = getPose() * t_mov;
            Transform t = getPose() * t_mov;
            //Send the new transform to ROS

            br.sendTransform(StampedTransform(t, Time::now(), "/map", name));


        }

       /* string getFar() {
            float time_to_wait = 0.1;

            StampedTransform trans1;
            StampedTransform trans2;
            StampedTransform trans3;

            ros::Time now = Time(0);

            try {

                listener.waitForTransform(name, "dcorreia", now, Duration(time_to_wait));
                listener.lookupTransform(name, "dcorreia", now, trans1);

                listener.waitForTransform(name, "jsousa", now, Duration(time_to_wait));
                listener.lookupTransform(name, "jsousa", now, trans2);

                listener.waitForTransform(name, "vsilva", now, Duration(time_to_wait));
                listener.lookupTransform(name, "vsilva", now, trans3);


            } catch (TransformException &ex) {

                ROS_ERROR("%s", ex.what());
                Duration(0.01).sleep();
            }

            float d1, d2, d3, distanciaEnimigo;
            string playerF;

            float x1 = trans1.getOrigin().x();
            float y1 = trans1.getOrigin().y();

            float x2 = trans2.getOrigin().x();
            float y2 = trans2.getOrigin().y();

            float x3 = trans3.getOrigin().x();
            float y3 = trans3.getOrigin().y();

            d1 = sqrt(x1 * x1 + y1 * y1);
            d2 = sqrt(x2 * x2 + y2 * y2);
            d3 = sqrt(x3 * x3 + y3 * y3);

            if (d1 < d2 && d1 < d3) {
                playerF = "dcorreia";
                distanciaEnimigo = d1;
            } else if (d2 < d3 && d2 < d1) {
                playerF = "jsousa";
                distanciaEnimigo = d2;
            } else {
                playerF = "vsilva";
                distanciaEnimigo = d3;
            }

            return playerF;

        }*/
        
        vector<string> getNear() {
            //tratamento para caçar
            float time_to_wait = 0.1;

            StampedTransform trans1,trans2,trans3,trans4,trans5,trans6;
            
            ros::Time now = Time(0);

            try {

                listener.waitForTransform(name, "fsilva", now, Duration(time_to_wait));
                listener.lookupTransform(name, "fsilva", now, trans1);

                listener.waitForTransform(name, "jferreira", now, Duration(time_to_wait));
                listener.lookupTransform(name, "jferreira", now, trans2);

                listener.waitForTransform(name, "rmartins", now, Duration(time_to_wait));
                listener.lookupTransform(name, "rmartins", now, trans3);
                //fugir
                
                listener.waitForTransform(name, "dcorreia", now, Duration(time_to_wait));
                listener.lookupTransform(name, "dcorreia", now, trans4);

                listener.waitForTransform(name, "jsousa", now, Duration(time_to_wait));
                listener.lookupTransform(name, "jsousa", now, trans5);

                listener.waitForTransform(name, "vsilva", now, Duration(time_to_wait));
                listener.lookupTransform(name, "vsilva", now, trans6);
                

            } catch (TransformException &ex) {

                ROS_ERROR("%s", ex.what());
                Duration(0.01).sleep();
            }

            float d1, d2, d3, d4, d5, d6, distanciaPresa,distanciaEnimigo;
                       
            vector<string> playerF;
            
            //string playerF[2];

            float x1 = trans1.getOrigin().x();
            float y1 = trans1.getOrigin().y();

            float x2 = trans2.getOrigin().x();
            float y2 = trans2.getOrigin().y();

            float x3 = trans3.getOrigin().x();
            float y3 = trans3.getOrigin().y();
            
            float x4 = trans4.getOrigin().x();
            float y4 = trans4.getOrigin().y();

            float x5 = trans5.getOrigin().x();
            float y5 = trans5.getOrigin().y();

            float x6 = trans6.getOrigin().x();
            float y6 = trans6.getOrigin().y();
            
            
            d1 = sqrt(x1 * x1 + y1 * y1);
            d2 = sqrt(x2 * x2 + y2 * y2);
            d3 = sqrt(x3 * x3 + y3 * y3);
            d4 = sqrt(x4 * x4 + y4 * y4);
            d5 = sqrt(x5 * x5 + y5 * y5);
            d6 = sqrt(x6 * x6 + y6 * y6);         
            
            
                if (d1 < d2 && d1 < d3) {
                    distanciaPresa = d1;
                } else if (d2 < d3 && d2 < d1) {
                    distanciaPresa = d2;
                } else {
                    distanciaPresa = d3;
                }
                        
                if (d4 < d5 && d4 < d6) {
                    distanciaEnimigo = d4;
                } else if (d5 < d6 && d5 < d4) {
                    distanciaEnimigo = d5;
                } else {
                    distanciaEnimigo = d6;
                }
                             
            if (distanciaEnimigo > distanciaPresa) {

                //playerF[1] = "apanha";
                playerF.push_back("apanha");
                
                if (d1 < d2 && d1 < d3) {
                    playerF.push_back("fsilva");
                    //distanciaPresa = d1;
                } else if (d2 < d3 && d2 < d1) {
                    //playerF[2] = "jferreira";
                    playerF.push_back("jferreira");
                    //distanciaPresa = d2;
                } else {
                    //playerF[2] = "rmartins";
                    playerF.push_back("rmartins");
                    //distanciaPresa = d3;
                }

            } else {
                    
                playerF.push_back("foge");
                        
                if (d4 < d5 && d4 < d6) {
                    //playerF[2] = "dcorreia";
                    playerF.push_back("dcorreia");
                    
                } else if (d5 < d6 && d5 < d4) {
                    //playerF[2] = "jsousa";
                    playerF.push_back("jsousa");
                } else {
                    //playerF[2] = "vsilva";
                    playerF.push_back("vsilva");
                }
            }
            return playerF;
        }

        void makeAPlayCallback(const rwsua2017_msgs::MakeAPlay::ConstPtr& msg) {

            vector<string> decisao = getNear();
            
            float turn_angle;
            
            if(decisao[0]=="apanha"){
                turn_angle =getAngleTo(decisao[1]);
                
            }else{
                turn_angle = getAngleTo(decisao[1]);
                turn_angle = (M_PI * 5 / 6) + turn_angle;
                }
            
            float displacement = msg->max_displacement;
            
            cout << "msg: max displacement -> " << msg->max_displacement << endl;

            move(displacement, msg->max_displacement, turn_angle, M_PI / 30);

            //enviar boca
            visualization_msgs::Marker marker;
            marker.header.frame_id = name;
            marker.header.stamp = ros::Time();
            marker.ns = name;
            marker.id = 0;
            marker.type = visualization_msgs::Marker::TEXT_VIEW_FACING;
            marker.action = visualization_msgs::Marker::ADD;
            marker.pose.position.x = 0;
            marker.pose.position.y = 0.4;
            marker.pose.position.z = 0;
            marker.pose.orientation.x = 0.0;
            marker.pose.orientation.y = 0.0;
            marker.pose.orientation.z = 0.0;
            marker.pose.orientation.w = 1.0;
            marker.scale.z = 0.4;
            marker.color.a = 1.0; // Don't forget to set the alpha!
            marker.color.r = 0.3;
            marker.color.g = 0.3;
            marker.color.b = 0.3;
            marker.frame_locked = 1;
            marker.lifetime = ros::Duration(1);
            marker.text = " :D ";
            vis_pub.publish(marker);

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

