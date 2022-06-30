#include <iostream>
#include "math.h" 
#include "ros/ros.h"
#include "geometry_msgs/PoseStamped.h"
#include "behavior_tree/behavior_tree.h"
#include "mavros_msgs/CommandTOL.h"
using namespace std; 

class Takeoff{
    private:
        ros::NodeHandle n;
        ros::ServiceClient mavros_srv;
        ros::Subscriber msg_sub_pose;
        float desired_height = 0;
    public: 
        bt::Action action_takeoff;
        Takeoff();
        void actionSet(int state);
        void cmdTakeoff();
};
Takeoff :: Takeoff() : action_takeoff("Takeoff"){
    mavros_srv = n.serviceClient<mavros_msgs::CommandTOL>("mavros/cmd/takeoff");
}

void Takeoff :: actionSet(int state){
    switch(state){
        case 1:
            action_takeoff.set_success();
            break;
        case 0:
            action_takeoff.set_running();
            break;
        case -1:
            action_takeoff.set_failure();
            break;
    }
    action_takeoff.publish();
    return;
}
void Takeoff :: cmdTakeoff(){
    mavros_msgs::CommandTOL srv_msg;
    n.getParam("drone/desired_height", desired_height);
    srv_msg.request.min_pitch = 0;
    srv_msg.request.yaw = 0;
    srv_msg.request.latitude = 0;
    srv_msg.request.longitude = 0;
    srv_msg.request.altitude = desired_height;
    if(mavros_srv.call(srv_msg)){
        ROS_INFO("Takeoff success");
    }
    else{
        ROS_INFO("Takeoff success");
    }
    actionSet(0);
    return;
}



int main(int argc, char **argv){
    ros::init(argc, argv, "mavros_cmd_takeoff");
    Takeoff mavros_takeoff;
    int tmp = 0;
    while(ros::ok()){
        if(mavros_takeoff.action_takeoff.is_active() && mavros_takeoff.action_takeoff.active_has_changed()){
            ROS_INFO("Action: Takeoff activiate");
            mavros_takeoff.cmdTakeoff();
        }
        if(mavros_takeoff.action_takeoff.active_has_changed() && !(mavros_takeoff.action_takeoff.is_active())){
            ROS_INFO("done");
            mavros_takeoff.actionSet(1);
        }
        if(mavros_takeoff.action_takeoff.is_active()){
            mavros_takeoff.actionSet(0);
        }
        ros::spinOnce();
    }
    return 0;
}

