Silbot3 source repository
===================

----------


How to setup in Ubuntu Environment
-------------


- ROS kinetic installation
- [ros wiki 참고](http://wiki.ros.org/kinetic/Installation/Ubuntu)

```sh
sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list'
sudo apt-key adv --keyserver hkp://ha.pool.sks-keyservers.net:80 --recv-key 0xB01FA116
sudo apt-get update
sudo apt-get install ros-kinetic-desktop
echo "source /opt/ros/kinetic/setup.bash" >> ~/.bashrc
source ~/.bashrc
```

- Install necessary packages
```sh
sudo apt install build-essential git python-gst-1.0 gstreamer1.0-libav ros-kinetic-move-base-msgs python-pyside.qtwebkit python-pyside.qtscript
```

- Create workspace

```sh
mkdir -p ~/robocare_ws/src
cd ~/robocare_ws/src
catkin_init_workspace
```

- Clone source repository

```sh
cd ~/robocare_ws/src
git clone http://192.168.112.102/silbot3/silbot3_src.git
```

- Build

```shell
cd ~/robocare_ws
catkin_make
```

- Environment setup

```shell
   echo "source ~/robocare_ws/devel/setup.bash" >> ~/.bashrc
   source ~/.bashrc
```
