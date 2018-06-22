#include "UltrasonicHandler.h"

using namespace cir::devicenode;

UltrasonicHandler::UltrasonicHandler() {
	_ultrasonicServerProxy = CUltrasonicERobot::getInstance();
	this->_ULTRA_THREAD_FLAG = false;
	initHandler();
	this->_this_thread = boost::thread(boost::bind(&UltrasonicHandler::ultraPublishThread, this));
}

UltrasonicHandler::~UltrasonicHandler() {
	this->_ULTRA_THREAD_FLAG = false;
	this->_this_thread.join();
	ROS_DEBUG("[UltrasonicHandler] closed... ");
}

void UltrasonicHandler::initHandler() {
	//this->_ultrasonicServiceServer = this->_nodeHandle.advertiseService("/DeviceNode/UltrasonicService", &UltrasonicHandler::ultrasonicServiceCallback, this);
	//ROS_INFO("UltraSonic handler created - /DeviceNode/UltrasonicService");
	this->_ultraPublisher = this->_nodeHandle.advertise<silbot3_msgs::Device_Ultra_Msg>("/DeviceNode/UltraSonic/data", 1000);
	ROS_INFO("UltraSonic handler created - /DeviceNode/UltraSonic/data");
}

bool UltrasonicHandler::ultrasonicServiceCallback(silbot3_msgs::Device_Ultra_Srv::Request &request, silbot3_msgs::Device_Ultra_Srv::Response &response) {

	string command = request.command;

	if(command == COMMAND_ULTRA_READ_DATA) {

		CUltrasonicData returnData = _ultrasonicServerProxy->readData(1);

		response.msg.param.push_back(returnData.getData()[0]);
		response.msg.param.push_back(returnData.getData()[1]);
		response.msg.param.push_back(returnData.getData()[2]);
		response.msg.param.push_back(returnData.getData()[3]);
		response.msg.param.push_back(returnData.getData()[4]);
		response.msg.param.push_back(returnData.getData()[5]);
		response.msg.param.push_back(returnData.getData()[6]);
		response.msg.param.push_back(returnData.getData()[7]);
		response.msg.param.push_back(returnData.getData()[8]);
		response.msg.param.push_back(returnData.getData()[9]);
		response.msg.param.push_back(returnData.getData()[10]);
		response.msg.param.push_back(returnData.getData()[11]);
		return true;
	} else {
		cout << "[UltrasonicHandler] unrecognized command. " << endl;
	}
	return false;
}

void UltrasonicHandler::ultraPublishThread() {
	this->_ULTRA_THREAD_FLAG = true;
	ros::Rate loop_rate(10);

	while( ros::ok() && this->_ULTRA_THREAD_FLAG ) {

		silbot3_msgs::Device_Ultra_Msg message;

		CUltrasonicData data = _ultrasonicServerProxy->readData(1);
		message.param.push_back(data.getData()[0]);
		message.param.push_back(data.getData()[1]);
		message.param.push_back(data.getData()[2]);
		message.param.push_back(data.getData()[3]);
		message.param.push_back(data.getData()[4]);
		message.param.push_back(data.getData()[5]);
		message.param.push_back(data.getData()[6]);
		message.param.push_back(data.getData()[7]);
		message.param.push_back(data.getData()[8]);
		message.param.push_back(data.getData()[9]);
		message.param.push_back(data.getData()[10]);
		message.param.push_back(data.getData()[11]);
		this->_ultraPublisher.publish(message);

		ros::spinOnce();
		loop_rate.sleep();
	}
}
