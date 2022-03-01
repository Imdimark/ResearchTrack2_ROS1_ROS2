/** @package rt2_assignment1
* 
* @file state_machine.cpp
* @brief this code implements the state_machine node 
* @author Giovanni Di Marco			
* @version 0.1
* @date 27/02/2022
*
*
*
* @details 
*
* Subscribes to: <BR>
*    None
* 
* Publishes to: <BR>
*    /cmd_vel
* 
* Client: <BR>
*    /position_server
* 	
* Actiom Client: <BR>
*    /reaching_goal
*
* Services: <BR>
*  /user_interface
*
* Description: 
*
* By means of an user interface, the user is able of making the robot starts 
* by entering the 1 integer value, the robot starts moving. There is one boolean 
* value which becomes true and then call the \verbatim position_service.cpp  \endverbatim
* which retrieves the random goal position to reach from the RandomPosition.srv custom 
* service, sends the random position as the action server goal, waits for the robot 
* to reach the designated position
*
*/
#include "ros/ros.h"
#include <geometry_msgs/Twist.h>
#include "rt2_assignment1/Command.h"
#include "rt2_assignment1/Position.h"
#include "rt2_assignment1/RandomPosition.h"
#include <rt2_assignment1/PlanningAction.h> 
#include <actionlib/client/simple_action_client.h> 
#include <rt2_assignment1/PlanningAction.h>
#include <actionlib/client/terminal_state.h>
#include <std_msgs/Float64.h>
#include <unistd.h>
#include <rt2_assignment1/PlanningAction.h>
bool start = false;
int stato = 0;

/**
 *@brief this is the callback of the server /user_interface.
 *@param req  request from the user through the choise made in the user_interface node 
 *@param res  
 * 
 *@retval boolean value
 * 
 * There are 3 states:
 * 0 --> initial state of the cicle
 * 1 --> start trigger has been received, in this phase a random position is reached and sent through a goal
 * 2 --> goal has ben sent, a feedback is received untill phase 3
 * 3 --> goal has been reached successfully
 * 
 * In this function the state 0 ( the start of the cicle) is allowed
 *  only if is the first iteration else if the goal is reached
 * else if the goal is cancelled
 * The stop command block the cicle, velocity will be zero and the goal is going to be deleted.
 */

bool user_interface(rt2_assignment1::Command::Request &req, rt2_assignment1::Command::Response &res){
    if (req.command == "start"){
    	start = true;
		if (stato == 3)
			{
				stato = 0;
			}
		
    }
    if (req.command == "stop") {
    	start = false;
    }
    return true;
}


/**
 * @brief Main function
 * 
 * @retval 0
 * 
 * This function declare: publisher, action, client and server used by the state machine.
 * a finite state machine, with 3 states has been implemented.
 * You can start, receive feedback and stop the simulation.
 * 
 * Description of the states:
 * From state 0 you can, through user command, go to state 1 and start the simulation or stay blocked.
 * From state 1 you can wait and reach the goal and turn back in state 0 in this way starts the next goal
 *  	or you can block the simulation (velocity set to 0 andcancel the goal) and turn back in state 0
 * From state 3 you can restart the cicle only if a start command is received
 */

int main(int argc, char **argv)
{
   ros::init(argc, argv, "state_machine");
   ros::NodeHandle n;
   ros::ServiceServer service= n.advertiseService("/user_interface", user_interface);
   ros::ServiceClient client_rp = n.serviceClient<rt2_assignment1::RandomPosition>("/position_server");
   ros::Publisher ResetVelPub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 1000);
   geometry_msgs::Twist msg;
   actionlib::SimpleActionClient<rt2_assignment1::PlanningAction> client("/reaching_goal", true);
   rt2_assignment1::RandomPosition rp;
   rp.request.x_max = 5.0;
   rp.request.x_min = -5.0;
   rp.request.y_max = 5.0;
   rp.request.y_min = -5.0;
   rt2_assignment1::Position p;
   rt2_assignment1::PlanningResultConstPtr result;
   int res;
   while(ros::ok()){
		ros::spinOnce();
			if (start ){
				if (stato == 0){
					printf ("entro qui");
					client_rp.call(rp);
					client.waitForServer();
					rt2_assignment1::PlanningGoal goal;	
					goal.target_pose.header.frame_id = "base_link";
					goal.target_pose.header.stamp = ros::Time::now();
					goal.target_pose.pose.position.x = rp.response.x;
					goal.target_pose.pose.position.y = rp.response.y;
					goal.target_pose.pose.orientation.w = rp.response.theta;	
					std::cout << "\nGoing to the position: x= " << goal.target_pose.pose.position.x << " y= " << goal.target_pose.pose.position.y << " theta = " <<goal.target_pose.pose.orientation.w << std::endl;
					ROS_INFO("Sending goal");
					client.sendGoal(goal);
					stato = 1;
					//client.waitForResult();
				}

				if (stato == 1){
					if(client.getState() == actionlib::SimpleClientGoalState::SUCCEEDED) {
						ROS_INFO("Hooray, target reached!");
						stato = 0;
					}
					else{
						ROS_INFO("i'm reaching the goal");
					}
				}			
			}
		if (start == false and stato == 1 ){
			printf ("entro a cancellare");
			client.cancelGoal();
			msg.linear.x=0;
			msg.linear.y=0;
			msg.angular.z=0;
			ResetVelPub.publish(msg);
			stato = 3;
		}
	 }  
   return 0;
}
