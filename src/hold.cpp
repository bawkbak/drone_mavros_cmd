#include <iostream>
#include <math.h> 
#include <ros/ros.h>
#include <behavior_tree/behavior_tree.h>
#include <mavros_msgs/SetMode.h>
using namespace std; 

class Action{
    private:
        ros::NodeHandle n;
        ros::ServiceClient mavros_srv;
    public: 
        bt::Action action;
        Action();
        void actionSet(int state);
        void cmdHold();
};
Action :: Action() : action("action_hold"){
    mavros_srv = n.serviceClient<mavros_msgs::SetMode>("mavros/set_mode");
}

void Action :: actionSet(int state){
    switch(state){
        case 1:
            action.set_success();
            break;
        case 0:
            action.set_running();
            break;
        case -1:
            action.set_failure();
            break;
    }
    action.publish();
    return;
}
void Action :: cmdHold(){
    mavros_msgs::SetMode srv_msg;
    srv_msg.request.custom_mode = "AUTO.LOITER";
    if(mavros_srv.call(srv_msg)){ return; }
    else{ ROS_INFO("Error"); }
    return;
}


int main(int argc, char **argv){
    ros::init(argc, argv, "mavros_cmd_hold");
    Action mavros_hold;
    while(ros::ok()){
        if(mavros_hold.action.is_active() && mavros_hold.action.active_has_changed()){
            ROS_INFO("Action: Action activiate");
        }
        if(mavros_hold.action.active_has_changed() && !(mavros_hold.action.is_active())){
            ROS_INFO("done");
            mavros_hold.actionSet(1);
        }
        if(mavros_hold.action.is_active()){
            mavros_hold.actionSet(0);
            mavros_hold.cmdHold();
            // ros::Duration(0.5).sleep(); 
        }
        ros::spinOnce();
    }
    return 0;
}