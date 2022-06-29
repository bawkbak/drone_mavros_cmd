#include <iostream>
#include "math.h" 
#include "ros/ros.h"
#include "geometry_msgs/PoseStamped.h"
#include "std_msgs/String.h"
#include "behavior_tree/behavior_tree.h"
#include <behavior_tree_msgs/Active.h>
#include "mavros_msgs/CommandBool.h"
using namespace std; 


class Init{
    private:
        ros::NodeHandle n;
        int state[4] = {0, 0, 0, 0};
        
    public: 
        bt::Condition condition_init;
        Init();
        void conditionSet(bool);
        void conditionPublish();
};

Init :: Init() : condition_init("A")
{}

void Init :: conditionSet(bool b){
    condition_init.set(b);
}

void Init :: conditionPublish(){
    condition_init.publish();
}


int main(int argc, char **argv){
    ros::init(argc, argv, "mavros_init");
    Init mavros_init;
    behavior_tree_msgs::Active test;
    int count = 0;
    while(ros::ok()){
        count++;
        std::cout << "count: " << count << endl;
        if(count > 25){
            mavros_init.conditionSet(false);
        }else{
            mavros_init.conditionSet(true);
        }
        mavros_init.conditionPublish();
        ros::Duration(0.3).sleep();
    }
    
    return 0;
}


