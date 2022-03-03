/**@package rt2_assignment1
* 
* @file position_service.cpp
* @brief this code implements the random_position_server node
* @author Giovanni Di Marco
* @version 0.1
* @date 27/02/2022
*
*
* @details 
*
* Subscribes to: <BR>
*    None
* 
* Publishes to: <BR>
*    None 
*
* Services: <BR>
*  /position_server
*
* Description: <BR>
*
* implements the random generation of a position, when it is triggered and receives in imput a minimum and a maximum
*
*/
#include "ros/ros.h"
#include "rt2_assignment1/RandomPosition.h"

/**
 * @brief function that generates randomic couple of numbers in the inteval M and N
 * @param M  minimum of the interval
 * @param N maximum of the interval
 * @retval random number, it is a double
 *
 */
double randMToN(double M, double N)
{     return M + (rand() / ( RAND_MAX / (N-M) ) ) ; }
/**
 * @brief Callback of the service, it is triggered
 * @param req  the request within the max and the min
 * @param res  the response
 * 
 * @retval the boolean True
 *  
 */

bool myrandom (rt2_assignment1::RandomPosition::Request &req, rt2_assignment1::RandomPosition::Response &res){
    res.x = randMToN(req.x_min, req.x_max);
    res.y = randMToN(req.y_min, req.y_max);
    res.theta = randMToN(-3.14, 3.14);
    return true;
}

/**
 * @brief  Main 
 * 
 * @retval 0
 * 
 * Here there are the initialization of the node and the ServiceServer, 
 * triggered by the client. The callback starts when the request 
 * has been done.
 
 */

int main(int argc, char **argv)
{
    /* init node */
   ros::init(argc, argv, "random_position_server");
   ros::NodeHandle n;
    /*ServiceServer is the name of the server,
    * /position_server is the name of the service and 
    * myrandom the name of the callback */
   ros::ServiceServer service= n.advertiseService("/position_server", myrandom);
   
   ros::spin();

   return 0;
}
