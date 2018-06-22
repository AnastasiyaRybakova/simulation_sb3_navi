/*
 * OdomPub.h
 *
 *  Created on: 2015. 6. 18.
 *      Author: ijpark
 */

#ifndef SRC_ODOMSETUP_H_
#define SRC_ODOMSETUP_H_


#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/LaserScan.h>
#include <boost/thread.hpp>

class OdomSetup {
public:
	OdomSetup();
	virtual ~OdomSetup();

	void velocityCB(const geometry_msgs::Twist::ConstPtr& msg);
	void publishOdom();
	void rpScanCB(const sensor_msgs::LaserScan::ConstPtr& msg);
	void sickScanCB(const sensor_msgs::LaserScan::ConstPtr& msg);
	void scanCB(const sensor_msgs::LaserScan::ConstPtr& msg);
	void threadFunction();

private:
	ros::NodeHandle n;
	ros::Subscriber sub;

	double velX, velY, velTh;

	ros::Subscriber rpSubscriber;
	ros::Subscriber sickSubscriber;
	ros::Subscriber subscriber;
	ros::Publisher scanPub;
	ros::Publisher baseScanPub;

	sensor_msgs::LaserScan scanMsg;
	sensor_msgs::LaserScan baseScanMsg;
	boost::thread			_this_thread;

	bool isScanUpdated;
};

#endif /* SRC_ODOMSETUP_H_ */
