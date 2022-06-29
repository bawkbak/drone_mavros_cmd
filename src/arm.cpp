#include <iostream>
#include "math.h" 
#include "ros/ros.h"
#include "ros/service_client.h"
#include "mavros_msgs/CommandBool.h"
#include <behavior_tree/behavior_tree.h>
using namespace std; 

class Arm{
    private:
        ros::NodeHandle n;
        ros::ServiceClient mavros_srv;
    public:
        bt::Action action_arm;
        Arm();
        void actionSet(int state);
        void cmdArming();
};

Arm :: Arm() : action_arm("Arm"){
    mavros_srv = n.serviceClient<mavros_msgs::CommandBool>("mavros/cmd/arming");
}

void Arm :: actionSet(int state){
    switch(state){
        case 1:
            action_arm.set_success();
            break;
        case 0:
            action_arm.set_running();
            break;
        case -1:
            action_arm.set_failure();
            break;
    }
    action_arm.publish();
    return;
}

void Arm :: cmdArming(){
    mavros_msgs::CommandBool srv_msg;
    srv_msg.request.value = true;
    if(mavros_srv.call(srv_msg)){
        ROS_INFO("Arm success");
    }
    else{
        ROS_INFO("Arm failed");
    }
    return;
}

int main(int argc, char **argv){
    ros::init(argc, argv, "mavros_cmd_arm");
    Arm mavros_arm;
    while(ros::ok()){
        if(mavros_arm.action_arm.is_active() && mavros_arm.action_arm.active_has_changed()){
            mavros_arm.actionSet(0);
            mavros_arm.cmdArming();
            mavros_arm.actionSet(1);
        }
        ros::spinOnce();
    }
    return 0;
}