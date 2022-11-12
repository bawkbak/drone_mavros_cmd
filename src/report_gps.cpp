#include <iostream> 
#include <ros/ros.h>
#include <behavior_tree/behavior_tree.h>
#include <sensor_msgs/NavSatFix.h>
using namespace std; 

class GPS{
    private:
        ros::NodeHandle n;
        ros::Subscriber sub_gps;
        float latitude, longitude;
    public: 
        bt::Action action;
        GPS();
        void stateCallback(const sensor_msgs::NavSatFix::ConstPtr& ms);
        void actionSet(int state);
        void reportGPS();
};
GPS :: GPS() : action("report_gps"){
    sub_gps = n.subscriber("mavros/global_position/global", 1,  &GPS::stateCallback, this);
}

void GPS :: stateCallback(const sensor_msgs::NavSatFix::ConstPtr& msg){
    latitude = msg->latitude;
    longitude = msg->longitude;
    return;
}

void GPS :: actionSet(int state){
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
void GPS :: reportGPS(){
    ROS_INFO("LATITUDE: %f", latitude);
    ROS_INFO("LONGITUDE: %f", longitude);
    return;
}


int main(int argc, char **argv){
    ros::init(argc, argv, "mavros_cmd_land");
    GPS report;
    int tmp = 0;
    while(ros::ok()){
        if(report.action.is_active() && report.action.active_has_changed()){
            // ROS_INFO("Action: GPS activiate");
        }
        if(report.action.active_has_changed() && !(report.action.is_active())){
            // ROS_INFO("done");
            report.actionSet(1);
        }
        if(report.action.is_active()){
            report.actionSet(0);
            report.reportGPS();
            // ros::Duration(0.5).sleep(); 
        }
        ros::spinOnce();
    }
    return 0;
}