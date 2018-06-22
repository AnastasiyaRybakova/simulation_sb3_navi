#include "TouchHandler.h"

using namespace cir::devicenode;

TouchHandler::TouchHandler() {
	_touchButtonServerProxy = new CTouchButtonERobot();
	this->_TOUCH_THREAD_FLAG = false;
	initHandler();
	this->_this_thread = boost::thread(boost::bind(&TouchHandler::touchPublishThread, this));
}

TouchHandler::~TouchHandler() {
	this->_TOUCH_THREAD_FLAG = false;
	this->_this_thread.join();
	ROS_DEBUG("[TouchHandler] closed... ");
}

void TouchHandler::initHandler() {
	this->_touchPublisher = this->_nodeHandle.advertise<silbot3_msgs::Device_Touch_Msg>("/DeviceNode/TouchButton/data", 1000);
	this->_touchErrorServiceServer = this->_nodeHandle.advertiseService("/DeviceNode/TouchErrorService", &TouchHandler::touchErrorServiceCallback, this);
	ROS_INFO("Touch handler created - /DeviceNode/TouchButton/data");
}

void TouchHandler::touchPublishThread() {
	ROS_INFO("[TouchHandler] start touchPublishThread... ");
	this->_TOUCH_THREAD_FLAG = true;
	ros::Rate loop_rate(10);

	while( ros::ok() && this->_TOUCH_THREAD_FLAG ) {
		silbot3_msgs::Device_Touch_Msg message;

		CTouchButtonData data = _touchButtonServerProxy->readData(0);
		message.param.push_back(data.getData()[0]);
		message.param.push_back(data.getData()[1]);
		message.param.push_back(data.getData()[2]);
		message.param.push_back(data.getData()[3]);
		message.param.push_back(data.getData()[4]);
		this->_touchPublisher.publish(message);
		//ros::spinOnce();
		loop_rate.sleep();
	}
}

bool TouchHandler::touchErrorServiceCallback(silbot3_msgs::Device_Error_Srv::Request &request, silbot3_msgs::Device_Error_Srv::Response &response) {

	string command = request.command;
	ROS_INFO("[TouchHandler] touchErrorServiceCallback receive a message - %s", command.c_str());

	int errorCode = _touchButtonServerProxy->detectingError(atoi(request.command.c_str()));
//	cout << errorCode << endl;
	response.iParams.push_back(errorCode);

	return RETURN_SUCCESS;
}
