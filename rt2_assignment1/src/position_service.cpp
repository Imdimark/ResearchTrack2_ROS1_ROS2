/**
* \file position_service.cpp
* \brief This files implements the servpos node and the service service_rand
* \author Giovanni Di Marco
* \version 0.1
* \date 27/02/2022
*
**/
#include <inttypes.h>
#include <memory>
#include <functional>
#include "rt2_assignment1/srv/random_position.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_components/register_node_macro.hpp"



using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;
using service3 = rt2_assignment1::srv::RandomPosition;


namespace rt2_assignment1{

/*! Position_Service Class:
*The architecture chosen to write the nodes of ROS2 is to create them as components, classes like this, that are loaded.
*This class implements the random generation of a position, when it is triggered and receives in imput a minimum and a maximum.
*/
class Position_Service : public rclcpp::Node
{
    /** Initialisation of the service. 
    * service_rand which is called by the FSM node
    */
    public:

      Position_Service(const rclcpp::NodeOptions & options)
      : Node("servpos", options)
      {
           service_rand = this->create_service<service3>("/position_server", std::bind(&Position_Service::handle_service, this, _1, _2, _3)); 
      }

    private:

     /**Handle_service function
    *
    * The request is sent, with a minimum and a maximum, when you need a new random location to reach.
    * @param request_header
    * @param request 
    * @param response 
    */
    void handle_service(
    const std::shared_ptr<rmw_request_id_t> request_header,
    const std::shared_ptr<service3::Request> request,
    const std::shared_ptr<service3::Response> response)
    {
        (void)request_header;
        
        response->x = request->x_min + (rand() / ( RAND_MAX / (request->x_max-request->x_min) ) );
        response->y = request->y_min + (rand() / ( RAND_MAX / (request->y_max-request->y_min) ) );
        response->theta =  -3.14+ (rand() / ( RAND_MAX / (6.28)));
    
      
    }
    rclcpp::Service<service3>::SharedPtr service_rand; 

};
}
RCLCPP_COMPONENTS_REGISTER_NODE(rt2_assignment1::Position_Service)
