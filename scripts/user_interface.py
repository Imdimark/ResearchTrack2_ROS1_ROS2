#! /usr/bin/env python

"""
.. module:: user_interface
    :platform: Unix
    :synopsis: this python code implement the interface with the user, it accepts imput from keybord or jupyter
.. moduleauthor:: Di Marco Giovanni
This node, user_interface, is the "middleman" between the finit state machine and the user
Subscribes to:
None
Publishes to:
None
Service :
/user_interface
"""
import rospy
import time
from rt2_assignment1.srv import Command
import roslib
roslib.load_manifest('rt2_assignment1')
import actionlib
from rt2_assignment1.msg import PlanningAction
from geometry_msgs.msg import Twist
import rt2_assignment1.msg

## Main function, description
#
# @var ui_client is the variable that defines the client of the service /user_interface
# @var x is the input from the user (keyboard or jupyter). Basically 1 for start and 0 for stop the simulation.

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
