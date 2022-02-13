# rt2_assignment1
Come lanciarlo:

Preparazione:
posizionare il package1 (link) dentro la workspace di ros1
posizionare il package 2 dentro la workspace di ros2
posizionare il package "ros1:bridge" dentro la workspace di ros2

catkin_make della workspace ros1
colcon buoid della workspace di ros2 (richiede molto tempo)

Lancio:
Aprire tre terminali:
```
1)source ros.sh quindi lanciare roslaunch rt2_assignment1 sim_interactionROS2.launch
2)source ros2.sh quindi lanciare ros2 launch rt2_assignment1 launch.py
3)source ros12.sh quindi lanciare   ros2 run ros1_bridge dynamic_bridge --bridge-all-topics
```


https://github.com/ros2/ros1_bridge
