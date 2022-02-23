# Research track 2 _ assignment 1

The branch action contain the same package in of the **main** branch, but with the go_to_point node modelled as
a ROS action server, instead of a “simple” server. 
Given that, the robot FSM node now implement mechanisms for possibly cancelling the goal,
when the related user command is received.

## Required packages:
* action branch of this repository --> ``` git clone https://github.com/Imdimark/rt2 -b action ```
* CoppeliaSim --> https://www.coppeliarobotics.com/downloads

## The description of the system
Inside the package you download from the **main** branch, there are the following folders:
1. **action** that contains the .action file, the action messages are generated automatically by it.
2. **launch** it contains two different files: sim.launch (for ROS1 simulation) and sim_interactionROS2.launch (if you use also the **ROS2** repository package)
3. **scripts** that contains the python code
4. **src** that contains the C++ code
5. **urdf** 
6. **srv** that contains the custom services

![Immagine-2022-02-23-213550](https://user-images.githubusercontent.com/78663960/155404294-9bd9a28f-3349-44f3-9810-2b8b5b29fa8d.jpg)

You will launch:
* the simulation environment (Gazebo)
* the node PositionServer, which implements a random position service
* the node GoToPoint, which implements an action to drive a robot toward a point in the environment
* the node FSM, which implements a service to start or stop the robot, a service to retrive the random position and calls the action 
* the UserInterface, which asks the user to start/stop the robot



## How to run:

``` 
source ros.sh
roscore &
cd name_workspace
catkinmake
roslaunch rt2_assignment1 sim.launch
```

In addition you can run the simulation in Copellia.
In an other shell:
``` 
source ros.sh
./copelliasim.sh 
```
then you open the scene "rt2_assignment1.ttt"

