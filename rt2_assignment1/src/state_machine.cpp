/**
* \file state_machine.cpp
* \brief This files implements the state_machine node, the client_gui and both the services service_ui and service_ui service_and
* \author Giovanni Di Marco
* \version 0.1
* \date 27/02/2022
*
**/

//#include "ros/ros.h"
#include "rt2_assignment1/srv/random_position.hpp"
#include "rt2_assignment1/srv/position.hpp"
#include "rt2_assignment1/srv/command.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_components/register_node_macro.hpp"

#include <chrono>
#include <cinttypes>
#include <memory>
#include <cstdlib>
#include <functional>
//#include "example_interfaces/srv/add_two_ints.hpp"
//using namespace std::chrono_literals;
//using AddTwoInts = example_interfaces::srv::AddTwoInts;

using service1 = rt2_assignment1::srv::Command;
using client1 = rt2_assignment1::srv::Position;
using client2 = rt2_assignment1::srv::RandomPosition;
using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

namespace rt2_assignment1{

/*! FSM Class:
*The architecture chosen to write the nodes of ROS2 is to create them as components, classes like this, that are loaded.
*/

class FSM : public rclcpp::Node
{
public:
/** Initialisation of the service and clients. 
	*service_ui interacting with the user UserInterface node, in particular with the commands from the user.
  *client_pos that communicates with the GoToPoint node, it sends the random position to start the simulation
  *client_rand that retrives from the PositionServer node the random position that has been generated
	*/
  FSM(const rclcpp::NodeOptions & options)
  : Node("state_machine", options)
  {
     service_ui = this->create_service<service1>(
    "/user_interface", std::bind(&FSM::handle_service, this, _1, _2, _3));
     client_pos = this->create_client<client1>("go_to_point");
    while (!client_pos->wait_for_service(std::chrono::seconds(1))){
     if (!rclcpp::ok()) {
      RCLCPP_ERROR(this->get_logger(), "client interrupted while waiting for service to appear.");
      return;
    }
    }
   /* RCLCPP_INFO(this->get_logger(), "waiting for service to appear...");
  }
   timer_ = this->create_wall_timer(
      2000ms, std::bind(&MinimalClient::call_server, this));
  } */

     client_rand = this->create_client<client2>("position_server");
    
    while (!client_rand->wait_for_service(std::chrono::seconds(1))){
     if (!rclcpp::ok()) {
      RCLCPP_ERROR(this->get_logger(), "client interrupted while waiting for service to appear.");
      return;
    }
   /* RCLCPP_INFO(this->get_logger(), "waiting for service to appear...");
  }
   timer_ = this->create_wall_timer(
      2000ms, std::bind(&MinimalClient::call_server, this)); */
  
    
  }
  
  }
   
     /**
    * finitestatemachine function:
    * The beaviour of this function is, in order:
    *1)It sends the request with the min and the max
    *2)It receive the result
    *3)It sends these values to the GoToPoint node in order to starts the robot simulation of the reaching goal
    */

    private:
    
    bool start = false;
    
    void finitestatemachine (){
    
                if (this->start){
	   	 auto req2 = std::make_shared<client2::Request>();
		
		 req2->x_max = 5.0;
		 req2->x_min = -5.0;
		 req2->y_max = 5.0;
		 req2->y_min = -5.0;	 
		 auto callback = [this](rclcpp::Client<client2>::SharedFuture future ) {
		         auto req = std::make_shared<client1::Request>();
		         this->response_rand=future.get();
		         
		         std::cout<<"received random numbers"<<std::endl;
		         std::cout<<this->response_rand->x<<std::endl;
		         
		         req->x = this->response_rand->x; //invia
		         req->y = this->response_rand->y;  //invia
		         req->theta = this->response_rand->theta; //invia
		         std::cout << "\nGoing to the position: x= " << req->x << " y= " <<req->y << " theta = " <<req->theta << std::endl; //reqx,y,theta = res2x,y,theta
		         auto response_received_callback = [this](rclcpp::Client<client1>::SharedFuture future) { //invio a go_to_point
		                                (void)future;
		                                std::cout << "Position reached" << std::endl;
		                                this->finitestatemachine();
            
                };
                
                auto resu = client_pos->async_send_request(req, response_received_callback);
           };
           auto future_result = client_rand->async_send_request(req2, callback);

	}
    }
     /**handle_service function 
    * 
    * The string command controls the activation of the function, basically it should be "start" or "stop"
    * 
    * @param request_header
    * @param request 
    * @param response 
    */
    void handle_service(
        const std::shared_ptr<rmw_request_id_t> request_header,
        const std::shared_ptr<service1::Request> request,
        const std::shared_ptr<service1::Response> response)
        {
            (void)request_header;
            
             if (request->command == "start"){
  
    	         this->start = true; 
    	         
    	         this->finitestatemachine();
              }
              else {
    	          this->start = false;
    	          
               }
             response->ok=true;  
             
         }
             
            /* RCLCPP_INFO(
             this->get_logger(),
              "request: %" PRId64 " + %" PRId64, request->a, request->b); */
    rclcpp::Service<service1>::SharedPtr service_ui; 
    rclcpp::Client<client1>::SharedPtr client_pos; 
    rclcpp::Client<client2>::SharedPtr client_rand; 
    std::shared_ptr<client2::Response> response_rand;
   
};
}
RCLCPP_COMPONENTS_REGISTER_NODE(rt2_assignment1::FSM) 
