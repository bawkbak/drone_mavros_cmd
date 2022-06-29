#!/usr/bin/env python
import rospy
from behavior_tree_msgs.msg import Active

def callback(data):
    rospy.loginfo("I heard %s", data.active)

    
def init():
    # In ROS, nodes are uniquely named. If two nodes with the same
    # name are launched, the previous one is kicked off. The
    # anonymous=True flag means that rospy will choose a unique
    # name for our 'listener' node so that multiple listeners can
    # run simultaneously.
    rospy.init_node('listener', anonymous=True)
    
    rospy.Subscriber("b_active", Active, callback)


if __name__ == '__main__':
    init()
    rospy.spin()