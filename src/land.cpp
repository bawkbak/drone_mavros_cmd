#include <iostream>
#include "math.h" 
#include "ros/ros.h"
#include <behavior_tree/behavior_tree.h>
#include "mavros_msgs/CommandTOL.h"
using namespace std; 


class Land{
    private:
        ros::NodeHandle n;
        ros::ServiceClient mavros_srv;
    public: 
        bt::Action action_land;
        Land();
        void actionSet(int state);
        void cmdLand();
};
Land :: Land() : action_land("action_land"){
    mavros_srv = n.serviceClient<mavros_msgs::CommandTOL>("mavros/cmd/land");
}

void Land :: actionSet(int state){
    switch(state){
        case 1:
            action_land.set_success();
            break;
        case 0:
            action_land.set_running();
            break;
        case -1:
            action_land.set_failure();
            break;
    }
    action_land.publish();
    return;
}
void Land :: cmdLand(){
    mavros_msgs::CommandTOL srv_msg;
    srv_msg.request.min_pitch = 0;
    srv_msg.request.yaw = 0;
    srv_msg.request.latitude = 0;
    srv_msg.request.longitude = 0;
    srv_msg.request.altitude = 0;
    if(mavros_srv.call(srv_msg)){ return; }
    else{ ROS_INFO("Error"); }
    return;
}


int main(int argc, char **argv){
    ros::init(argc, argv, "mavros_cmd_land");
    Land mavros_land;
    int tmp = 0;
    while(ros::ok()){
        if(mavros_land.action_land.is_active() && mavros_land.action_land.active_has_changed()){
            ROS_INFO("Action: Land activiate");
        }
        if(mavros_land.action_land.active_has_changed() && !(mavros_land.action_land.is_active())){
            ROS_INFO("done");
            mavros_land.actionSet(1);
        }
        if(mavros_land.action_land.is_active()){
            mavros_land.actionSet(0);
            mavros_land.cmdLand();
            // ros::Duration(0.5).sleep(); 
        }
        ros::spinOnce();
    }
    return 0;
}