/*
 * OdomPub.cpp
 *
 *  Created on: 2015. 6. 18.
 *      Author: ijpark
 */

#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/LaserScan.h>
#include <reactive/erobot/ERobotCore.h>
#include <robocare/device/common/WheelEncoder.h>
#include <device/idif/wheelencoder/WheelEncoderERobot.h>
#include <device/idif/gyro/GyroERobot.h>
#include "OdomSetup.h"
#include <stdlib.h>

OdomSetup::OdomSetup(){
	this->n = n;
	this->sub = this->n.subscribe<geometry_msgs::Twist>("/cmd_vel", 1000,	&OdomSetup::velocityCB, this);

	rpSubscriber = this->n.subscribe<sensor_msgs::LaserScan>("/rp/scan", 1000, &OdomSetup::rpScanCB, this);
	sickSubscriber = this->n.subscribe<sensor_msgs::LaserScan>("/sick/scan", 1000, &OdomSetup::sickScanCB, this);
	subscriber = this->n.subscribe<sensor_msgs::LaserScan>("/scan", 1000, &OdomSetup::scanCB, this);
	scanPub = this->n.advertise<sensor_msgs::LaserScan>("/scan", 1000);
	baseScanPub = this->n.advertise<sensor_msgs::LaserScan>("/base_scan", 1000);

	this->_this_thread = boost::thread(boost::bind(&OdomSetup::threadFunction, this));

	this->isScanUpdated = false;
}

OdomSetup::~OdomSetup() {
}

void OdomSetup::velocityCB(const geometry_msgs::Twist::ConstPtr& msg) {

	ROS_DEBUG("cmd_vel subscribed : %f, %f, %f", msg->linear.x, msg->linear.y,	msg->angular.z);

	velX = msg->linear.x;
	velY = 0;
	velTh = msg->angular.z;

}

void OdomSetup::threadFunction() {
	ROS_DEBUG("OdomSetup::threadFunction Started");
	publishOdom();
}

void OdomSetup::scanCB(const sensor_msgs::LaserScan::ConstPtr& msg) {
	baseScanMsg = *msg;
}

//header:
//  seq: 747
//  stamp:
//    secs: 1443600185
//    nsecs: 661902191
//  frame_id: rplidar_link
//angle_min: 3.14159274101
//angle_max: -3.12413907051
//angle_increment: -0.0174532923847
//time_increment: 5.45648276784e-07
//scan_time: 0.000195887725567
//range_min: 0.15000000596
//range_max: 6.0


void OdomSetup::rpScanCB(const sensor_msgs::LaserScan::ConstPtr& msg) {
	isScanUpdated = true;
	scanMsg.ranges.clear();
	scanMsg.header.frame_id = "laser_link";
	scanMsg.header.seq = msg->header.seq;
	scanMsg.header.stamp.sec = msg->header.stamp.sec;
	scanMsg.header.stamp.nsec = msg->header.stamp.nsec;
	scanMsg.angle_increment = msg->angle_increment;
	scanMsg.time_increment = msg->time_increment;
	scanMsg.scan_time = msg->scan_time;
	scanMsg.range_min = msg->range_min;
	scanMsg.range_max = msg->range_max;

	scanMsg.angle_min = M_PI/2;
	scanMsg.angle_max = -M_PI/2;

	for(int i=270; i<360; i++) {
		scanMsg.ranges.push_back(msg->ranges[i]);
	}

	for(int i=0; i<90; i++) {
		scanMsg.ranges.push_back(msg->ranges[i]);
	}
	//this->scanMsg = *msg;

//	scanMsg.header.stamp = ros::Time::now();
//	scanPub.publish(scanMsg);

}


void OdomSetup::sickScanCB(const sensor_msgs::LaserScan::ConstPtr& msg) {
	isScanUpdated = true;

	scanMsg.ranges.clear();
	scanMsg.header.frame_id = "laser_link";
	scanMsg.header.seq = msg->header.seq;
	scanMsg.header.stamp.sec = msg->header.stamp.sec;
	scanMsg.header.stamp.nsec = msg->header.stamp.nsec;
	scanMsg.angle_increment = msg->angle_increment;
	scanMsg.time_increment = msg->time_increment;
	scanMsg.scan_time = msg->scan_time;
	scanMsg.range_min = msg->range_min;
	scanMsg.range_max = msg->range_max;

	scanMsg.angle_min = msg->angle_min;
	scanMsg.angle_max = msg->angle_max;

	scanMsg.ranges = msg->ranges;		
//	this->scanMsg = *msg;

//	ROS_WARN("size : %d",  msg->ranges.size());
//	cout << msg->ranges.size() << endl;
/*
	scanMsg.ranges.push_back(msg->ranges[0]);
	scanMsg.ranges.push_back(msg->ranges[1]);
	int size = 171;
	for(int i=2; i<size-2 ; i++) {
		float dm2 = msg->ranges[i-2];
		float dm1 = msg->ranges[i-1];
		float data = msg->ranges[i];
		float dp1 = msg->ranges[i+1];
		float dp2 = msg->ranges[i+2];


//		if(data < 1.0 && data > 0.001) {
//			cout << i << "]]]"<<dm2 << ", " << dm1 << ", " << data << ", " <<dp1 << ", " <<dp2 <<  endl;
//			cout << i << "]]]"<<msg->intensities[i-2] << ", " << msg->intensities[i-1] << ", " << msg->intensities[i] << ", " <<msg->intensities[i+1] << ", " <<msg->intensities[i+2] <<  endl;
//		}

		if(abs(dm2-dm1) <0.05 && abs(dp2-dp1)< 0.05) {
			if(abs(dm1-data) > 0.30 && abs(dp1-data) > 0.30) {
//				cout << "modify type 1  : " << i << "}}}"<< dm2 << ", " << dm1 << ", " << data << ", " <<dp1 << ", " <<dp2 <<  endl;
				scanMsg.ranges.push_back(dm1);
			} else {
				scanMsg.ranges.push_back(data);
			}
		}
		else if(abs(dp1-dm1) < 0.1 ) {
			if(abs(dm1-data) > 0.30 && abs(dp1-data) > 0.30) {
//				cout << "modify type 2 : " << i << "}}}"<< dm2 << ", " << dm1 << ", " << data << ", " <<dp1 << ", " <<dp2 <<  endl;
				scanMsg.ranges.push_back(dm1);
			} else {
				scanMsg.ranges.push_back(data);
			}
		}
		else {
			scanMsg.ranges.push_back(data);
		}


	}

	scanMsg.ranges.push_back(msg->ranges[size-2]);
	scanMsg.ranges.push_back(msg->ranges[size-1]);
*/

	scanPub.publish(scanMsg);
}

void OdomSetup::publishOdom() {

	ERobotCore::getInstance()->waitForWheelUpdate();
	ROS_INFO("publish odom started");

	ros::Publisher odom_pub = n.advertise<nav_msgs::Odometry>("/odom", 50);
	tf::TransformBroadcaster tf_broadcaster;

	double x = 0.0;
	double y = 0.0;
	double th = 0.0;

	double vx = velX;
	double vy = velY;
	double vth = velTh;

	ros::Time current_time, last_time;
	current_time = ros::Time::now();
	last_time = ros::Time::now();

	//CWheelEncoder* encoder = CWheelEncoderClientProxy::newInstance();


	double x0, y0, t0;
	x0 = y0 = t0;
	CWheelEncoder* encoder = CWheelEncoderERobot::getInstance();
	CGyro* gyro = CGyroERobot::getInstance();
	CWheelEncoderData d = encoder->readData(0);
	CGyroData gd = gyro->readData();
	x0 = d.getX() / 1000.;
	y0 = d.getY() / 1000.;
	t0 = d.getTheta() / 180. * M_PI;

	ROS_DEBUG("%f, %f, %f", x0, y0, t0);



	CWheelEncoderData ed = encoder->readData(0);

	//
	// initial value setup
	//


	double XRcurrent = 0.0;
	double YRcurrent = 0.0;
	double TRcurrent = 0.0;
	double XRprev = 0;
	double YRprev = 0;
	double TRprev = 0;

	double xn_1, xn, yn_1, yn;
	xn_1 = xn = yn_1 = yn = 0.;

	ros::Rate r(20);
	ROS_INFO("ROBOT setup TF published");
	while (n.ok()) {
		d = encoder->readData(0);
		gd = gyro->readData();
		XRcurrent = d.getX() / 1000. - x0;
		YRcurrent = d.getY() / 1000. - y0;
//		TRcurrent = d.getTheta() / 180. * M_PI - t0;
		TRcurrent = gd.getAngle()/ 180. * M_PI;
		//cout << "TRcurrent : " << TRcurrent << endl;

		double dx = XRcurrent - XRprev;
		double dy = YRcurrent - YRprev;
		double dt = TRcurrent - TRprev;


		xn = xn_1 + cos(TRcurrent) * dx - sin(TRcurrent) * dy;
		yn = yn_1 + sin(TRcurrent) * dx + cos(TRcurrent) * dy;


//		xn = (xn_1 + sqrt(dx*dx + dy*dy) * cos(TRcurrent));
//		yn = (yn_1 + dx * sin(TRcurrent));



		tf_broadcaster.sendTransform(
	      tf::StampedTransform(
	        tf::Transform(tf::Quaternion(0, 0, 0, 1), tf::Vector3(0.15, 0.0, 0.2)),
	        ros::Time::now(),"base_link", "laser_link"));

//		 tf::Quaternion rpldarQ;
//		 rpldarQ.setRPY(0, 0, 3.14);
//
//		tf_broadcaster.sendTransform(
//			      tf::StampedTransform(
//			        tf::Transform(rpldarQ, tf::Vector3(0.1, 0.0, 0.2)),
//			        ros::Time::now(),"base_link", "rplidar_link"));

		tf_broadcaster.sendTransform(
	      tf::StampedTransform(
	        tf::Transform(tf::Quaternion(0, 0, 0, 1), tf::Vector3(0.1, 0.0, 0.2)),
	        ros::Time::now(),"base_link", "camera_link"));

		tf_broadcaster.sendTransform(
	      tf::StampedTransform(
	        tf::Transform(tf::Quaternion(0, 0, 0, 1), tf::Vector3(0.0, 0.0, 0.0)),
	        ros::Time::now(),"base_link", "base_footprint"));

		tf_broadcaster.sendTransform(
	      tf::StampedTransform(
	        tf::Transform(tf::Quaternion(0, 0, 0, 1), tf::Vector3(0.0, 0.0, 0.0)),
	        ros::Time::now(),"base_link", "ultra_link"));


		geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromYaw(TRcurrent);

		geometry_msgs::TransformStamped odom_trans;
		odom_trans.header.stamp = ros::Time::now();
		odom_trans.header.frame_id = "odom";
		odom_trans.child_frame_id = "base_link";
		//odom_trans.child_frame_id = "base_foot_print";
		odom_trans.transform.translation.x = xn;
		odom_trans.transform.translation.y = yn;
		odom_trans.transform.translation.z = 0.0;
		odom_trans.transform.rotation = odom_quat;

		tf_broadcaster.sendTransform(odom_trans);

		//next, we'll publish the odometry message over ROS
		nav_msgs::Odometry odom;
		odom.header.stamp = ros::Time::now();
		odom.header.frame_id = "odom";
		//set the position
		odom.pose.pose.position.x = xn;
		odom.pose.pose.position.y = yn;
		odom.pose.pose.position.z = 0.0;
		odom.pose.pose.orientation = odom_quat;
		//cout<<odom_quat
		//set the velocity
		odom.child_frame_id = "base_link";
		odom.twist.twist.linear.x = velX;
		odom.twist.twist.linear.y = velY;
		odom.twist.twist.angular.z = velTh;
		//publish the message
		odom_pub.publish(odom);
		ROS_DEBUG("ODOM published (%f, %f, %f)", xn, yn, TRcurrent);

		//
		// laser scan publish to base_scan
		//
		//
//		if(isScanUpdated) {
//
//			scanPub.publish(scanMsg);
//			isScanUpdated = false;
//		}

		baseScanPub.publish(baseScanMsg);

		xn_1 = xn;
		yn_1 = yn;

		XRprev = XRcurrent;
		YRprev = YRcurrent;
		TRprev = TRcurrent;

		r.sleep();
		ros::spinOnce();
	}
}
