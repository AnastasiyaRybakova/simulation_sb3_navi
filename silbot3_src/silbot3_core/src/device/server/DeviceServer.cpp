#include "DeviceServer.h"
#include <ros/ros.h>

using namespace cir::devicenode;

DeviceServer::DeviceServer() {
	this->initRobot();
}

DeviceServer::~DeviceServer() {

}

void DeviceServer::setPantiltHome() {
	CPanTiltERobot* pantilt = new CPanTiltERobot;
	pantilt->setHomePosition();
}

void DeviceServer::initNode(int argc, char** argv) {
	ArmHandler	armHandler;
	GyroHandler gyroHandler;
	UltrasonicHandler ultrasonicHandler;
	WheelEncoderHandler wheelencoderHandler;
	WheelHandler wheelHandler;
	LEDHandler ledHandler;
	TouchHandler touchHandler;
	PantiltHandler pantiltHandler;
	BatteryHandler batteryHandler;

	OdomSetup odomSetup;
	JointSetup jointSetup;

	ROS_INFO("silbot3_core launching complete - Ver0.0.2");
	ros::spin();
}

void DeviceServer::initRobot() {
	bool checkMotor;
	bool checkVersion;
	ros::NodeHandle nh("~");

	nh.param<bool>("core/check_motor_error", checkMotor, false);
	nh.param<bool>("core/check_version", checkVersion, false);

	if (checkMotor)
	{
			ERobotCore::getInstance()->checkMotorError();
	}

	if (checkVersion)
	{
			ERobotCore::getInstance()->checkVersion();
	}

	ERobotCore::getInstance()->initERobotCore();
}

int main(int argc, char** argv) {
	cout << "start silbot3_core_node" <<endl;

	ros::init(argc, argv, "silbot3_core_node");

	DeviceServer server;
	server.initNode(argc,argv);

	return 0;
}
