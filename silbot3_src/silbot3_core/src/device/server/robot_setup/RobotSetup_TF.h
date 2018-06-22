/*
 * RobotSetup.h
 *
 *  Created on: 2015. 8. 11.
 *      Author: ijpark
 */

#ifndef SRC_DEVICE_SERVER_ROBOT_SETUP_ROBOTSETUP_TFH_
#define SRC_DEVICE_SERVER_ROBOT_SETUP_ROBOTSETUP_TFH_

#include <ros/node_handle.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/LaserScan.h>


namespace device {
namespace server {

class RobotSetup_TF {
public:
	RobotSetup_TF();
	virtual ~RobotSetup_TF();


	void setup(const ros::NodeHandle& nh);
};

} /* namespace server */
} /* namespace device */

#endif /* SRC_DEVICE_SERVER_ROBOT_SETUP_ROBOTSETUP_TFH_ */
