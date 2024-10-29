# The kingheron workspace

ROS melodic workspace using Clearpath heron (2018) packages to bring the Clearpath kingfisher (2013) back.

This workspace contains a obstacle avoidance package (heron mission) whose design and implementation are parts of a Dalhousie University co-op project in the Intelligent Systems Laboratory (ISL).

## Get starting

### Network

A local network is created using the Clearpath RTK base station (a yellow box with an big white antenna on top of it).

- connect the base station to a power supply (connect to a power socket or a battery)
- an ethernet cable can connect the base station to the ISP using its WAN port (on the ISP network, the base station has ```IP=129.173.86.206```)
- base station network : ```SSID=BASE_STATION_RTK``` & ```password=clearpath```
- the base station can be configured at this address ```192.168.1.1```

### Kingfisher

To run Ubuntu 18.04 and ROS melodic, the original Fit-PC-2i computer has been replaced by an NVIDIA Jetson nano. This new embedded computer need a 5V & 2A power supply, it can be connect to a monitor using an HDMI cable, and to a keyboard & a mouse throught USB ports.

#### Communication

Communication with the robot is achieved by connecting kingfisher's USB hub to the Jetson nano.

Since the Jetson nano haven't any built-in WIFI interface, a D-Link DWA-171 AC600 MU-MIMO Wi-Fi USB adapter has been installed using [this](https://gist.github.com/TOTON95/b445509399a0d0314d2bc4079527f5a8) driver.

#### The current setup

The micro SD card and the Wi-Fi USB adapter can be find the Kingfisher box. The Ubuntu's password for the main user ```kingheron``` is ```vect```.

#### Create a new setup

On a fresh Jetson nano with an official OS installation
- install ROS melodic
- install additional packages : ```sudo apt install []```
  - python-rosintall
  - python-rosintall-generator
  - python-wstool
  - python-rosdep
  - build-essential
- install workspace dependencies : ```sudo apt install ros-melodic-[]```
  - rosserial-server
  - app-manager
  - driver-base
  - angles
  - cmake-modules
  - robot-upstart
  - lms1xx
  - wireless-watcher
  - um6
  - imu-filter-madgwick
  - nmea-comms
  - nmea-navsat-driver
  - robot-localization
  - rqt
- clone the workspace : ```cd ~ & git clone https://github.com/Jules156/kingheron_ws.git```
- compile & source the workspace : ```cd ~/kingheron_ws & catkin_make & source devel/setup.bash```
- define device rules (create aliases for the kingfisher's sensors & actuators) : ```rosrun heron_bringup setup```
- create a job named 'ros' that will launch the project's nodes at boot time : ```rosrun heron_bringup install```

#### SSH connection

From another computer connected on the base station's network
- ```ssh kingheron@192.168.1.111```
- ```source ~/kingheron/setenvar```

## Controler packages

All of the parameters of the following packages can be adjusted using ```.yaml``` file in their respective ```config``` folder.

### Heron controller

In DEFAULT mode, the controller send a ```cmd_drive``` correspondind to a zero command for each motor. This topic can't be overwrite by publishing on ```cmd_drive```.

To activate low control loop one can publish on the following topics
- ```cmd_wrench``` : force[3] & torque[3] mechanical action
- ```cmd_course``` : speed in m/s (between -1.8 & +2.7) & yaw in radians
- ```cmd_helm``` : thrust in percents of thrusters power & yaw_rate in rad/s

Speed, yaw & yaw_rate control are achieved using fine tuned PID controllers.

### Heron mission

The ```heron_mission``` package propose 2 types of mission.

#### GPS point mission

By calling the ```/planner/gps_mission``` service one can ask the kingfisher to go at a given GPS position.

#### DWA mission

By calling the ```/planner/dwa_mission``` service one can ask the kingfisher to head for the a goal point while avoiding obstacles detected by the LIDAR. It uses the a custom implementation of the Dynamic Window Approach algorithm.

## Sensing mechanism

- IMU
- GPS, NMEA & navsat
- LIDAR
