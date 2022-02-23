# Research track 2 _ assignment 1


## Required packages:
* main branch of this repository --> ``` git clone https://github.com/Imdimark/rt2 -b main
* CoppeliaSim --> https://www.coppeliarobotics.com/downloads

## The description of the system
Inside the package you download from the **main** branch, there are the following folders:
1. **launch** it contains two different files: sim.launch (for ROS1 simulation) and sim_interactionROS2.launch (if you use also the **ROS2** repository package)
2. **scripts** that contains the python code
3. **src** that contains the C++ code
4. **urdf** 
5. **srv** that contains the custom services

![Immagine-2022-02-23-213550](https://user-images.githubusercontent.com/78663960/155404294-9bd9a28f-3349-44f3-9810-2b8b5b29fa8d.jpg)

You will launch:
* the simulation environment (Gazebo)
* the node PositionServer, which implements a random position service
* the node GoToPoint, which implements a service to drive a robot toward a point in the environment
* the node FSM, which implements a service to start or stop the robot, and calls the other two services to drive the robot
* the UserInterface, which asks the user to start/stop the robot, and calls the service implemented in the FSM node



## How to run:

``` 
source ros.sh
roscore &
cd name_workspace
catkinmake
roslaunch 
```

In addition you can run the simulation in Copellia.
In an other shell:
``` 
source ros.sh
./copelliasim.sh 
```
then you open the scene "rt2_assignment1.ttt"
