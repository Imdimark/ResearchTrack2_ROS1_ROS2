# Research track 2 _ assignment 1


## Required packages:
* main branch of this repository --> ``` git clone https://github.com/Imdimark/rt2 -b main ```
* ROS2 branch of this repository --> ``` git clone https://github.com/Imdimark/ROS2 -b main ```
* ros2/ros1_bridge -->``` git clone https://github.com/ros2/ros1_bridge ```

## The description of the system
![Immagine-2022-02-23-213550](https://user-images.githubusercontent.com/78663960/155404294-9bd9a28f-3349-44f3-9810-2b8b5b29fa8d.jpg)


The system consists of three parts:
### ROS1
Inside the package you download from the **main** branch, there are the following folders:
1. **scripts** that contains the python code

### ROS2
Inside the package you download from the **ROS2** branch, there are the following folders:
2. **launch** it contains two different files: sim.launch (for ROS1 simulation) and sim_interactionROS2.launch (if you use also the **ROS2** repository package)
3. **src** that contains the C++ code
4. **urdf** 
5. **srv** that contains the custom services
### BRIDGE
A network bridge which enables the exchange of messages between ROS 1 and ROS 2.

You will launch:
* the simulation environment (Gazebo)
* the node PositionServer, which implements a random position service
* the node GoToPoint, which implements a service to drive a robot toward a point in the environment
* the node FSM, which implements a service to start or stop the robot, and calls the other two services to drive the robot
* the UserInterface, which asks the user to start/stop the robot, and calls the service implemented in the FSM node
* the bridge

The two images below represent ROS1 nodes and ROS2 nodes respectively
![rosgraph_1](https://user-images.githubusercontent.com/78663960/155629183-772cb67b-4e40-4a29-ad06-a05673b67f34.png)
![rosgraph_2](https://user-images.githubusercontent.com/78663960/155629200-596f122c-f054-4b10-b459-383392496b89.png)



## How to run:
Put the package form the main branch inside the ROS1 workspace and the other 2 package required inside the ROS2 workspace.


Then open three shell and launch:

1) 
``` 
source ros.sh
roscore &
catkin_make
roslaunch rt2_assignment1 sim_interactionROS2.launch
```

2) 
``` 
source ros2.sh
colcon build 
ros2 launch rt2_assignment1 launch.py
```

3) 
``` 
source ros12.sh
ros2 run ros1_bridge dynamic_bridge
```
Alternatively you can run the scritpt **all_simulation.sh**
