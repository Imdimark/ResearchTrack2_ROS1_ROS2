# Research track 2 
## assignment 1

Inside the package:
1. Launch folder, it contains two different files:
* sim.launch
2. Script folder
3. Src folder
4. Urdf

You will launch:
* the simulation environment
* the node PositionServer, which implements a random position service
* the node GoToPoint, which implements a service to drive a robot toward a point in the environment
* the node FSM, which implements a service to start or stop the robot, and calls the other two services to drive the robot
* the UserInterface, which asks the user to start/stop the robot, and calls the service implemented in the FSM node




![Immagine-2022-02-23-213550](https://user-images.githubusercontent.com/78663960/155404294-9bd9a28f-3349-44f3-9810-2b8b5b29fa8d.jpg)


You can then run:
roslaunch rt2_assignment1 sim.launch
The launch file will open:
- a Gazebo simulation with a simple mobile robot
- four nodes which implement the robot’s behaviourv

## How to run:

``` 
Source ros.sh
roscore &
cd name_workspace
catkinmake
roslaunch 
```

In an other shell:
``` 
Source ros.sh
roscore &
./copelliasim.sh 
```
