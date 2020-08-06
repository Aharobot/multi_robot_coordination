# Multi-Robot Coordination

This work is described in the publication _"Multi-robot Coordination with Agent-Server Architecture for Autonomous Navigation in Partially Unknown Environments" by Luca Bartolomei, Marco Karrer and Margarita Chli, IROS 2020_.  
The video can be found [here](https://www.youtube.com/watch?v=BlFbiuV-d10).  

**This repo is under active maintenance.**

## Installation instructions
Install Ubuntu 18.04 and ROS Melodic. Install these dependencies:
```
$ sudo apt install python-catkin-tools python-wstool ros-melodic-joy ros-melodic-octomap-ros protobuf-compiler libgoogle-glog-dev ros-melodic-mav-msgs ros-melodic-mav-planning-msgs ros-melodic-sophus ros-melodic-hector-gazebo-plugins ros-melodic-pcl-ros ros-melodic-pcl-conversions libatlas-base-dev python-matplotlib python-numpy
$ sudo apt install liblapacke-dev libode6 libompl-dev libompl12 libopenexr-dev libglm-dev
$ sudo apt install clang-format
```  

Create a `catkin_ws` folder:
```
$ mkdir -p catkin_ws/src
$ cd catkin_ws
```
Set-up the workspace:
```
$ catkin init
$ catkin config --extend /opt/ros/melodic
$ catkin config --cmake-args -DCMAKE_BUILD_TYPE=Release
$ catkin config --merge-devel
```

Clone the dependencies:
```
$ cd ~/catkin_ws/src
$ wstool init
$ wstool merge ar_planning/dependencies.rosinstall
$ wstool up -j8
```

Remove the useless packages:
```
$ touch mav_control_rw/mav_linear_mpc/CATKIN_IGNORE
$ rm -rf ewok/catkin_simple/
$ rm -rf ewok/mav_comm/
$ rm -rf ewok/rotors_simulator/
```  

Finally, build the workspace:
```
$ cd ~/catkin_ws
$ catkin build
```  

## Troubleshooting
* If there are problems with `OMPL`, make sure that the `ROS` version is not installed: `sudo apt remove ros-melodic-ompl`.  

## Contributing
Contributions that help improving the code are welcome. In case you want to contribute, please adapt to the [Google C++ coding style](https://google.github.io/styleguide/cppguide.html/) and run `bash clang-format-all .` on your code before committing.
