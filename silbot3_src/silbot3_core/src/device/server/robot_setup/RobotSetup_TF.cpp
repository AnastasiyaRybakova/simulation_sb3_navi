/*
 * RobotSetup.cpp
 *
 *  Created on: 2015. 8. 11.
 *      Author: ijpark
 */

#include "RobotSetup_TF.h"
#include "OdomSetup.h"
#include <boost/thread.hpp>

namespace device {
namespace server {


RobotSetup_TF::RobotSetup_TF() {
}

RobotSetup_TF::~RobotSetup_TF() {
}


void RobotSetup_TF::setup(const ros::NodeHandle& nh) {
	OdomSetup os(nh);
	boost::thread odomThread = boost::thread(boost::bind(&OdomSetup::publishOdom, &os));
	std::cout << "\n\n\n\n\ RobotSetup_TF setup... \n\n\n\n" << std::endl;
}

} /* namespace server */
} /* namespace device */
