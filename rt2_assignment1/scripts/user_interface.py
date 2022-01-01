#! /usr/bin/env python
import rospy
import time
from rt2_assignment1.srv import Command
import roslib
roslib.load_manifest('rt2_assignment1')
import actionlib
from rt2_assignment1.msg import PlanningAction
from geometry_msgs.msg import Twist
import rt2_assignment1.msg
def main():
    rospy.init_node('user_interface')
    ui_client = rospy.ServiceProxy('/user_interface', Command)   
    time.sleep(10)
    rate = rospy.Rate(20)
    x = int(input("\nPress 1 to start the robot, 0 to stop the robot "))
    while not rospy.is_shutdown():
        if (x == 1):
            ui_client("start")
            x = int(input("\nPress 0 to stop the robot "))
        if (x == 0):
            ui_client("stop")            
            x = int(input("\nPress 1 to restart the robot "))
if __name__ == '__main__':
    main()
