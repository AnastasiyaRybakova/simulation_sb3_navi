#include "LEDHandler.h"

using namespace cir::devicenode;

LEDHandler::LEDHandler() {
	initHandler();
	_ledServerProxy = new CLightEmittingDiodeERobot();
}

LEDHandler::~LEDHandler() {

}

void LEDHandler::initHandler() {

	this->_ledSubscriber = this->_nodeHandle.subscribe<silbot3_msgs::Device_LED_Msg>("/DeviceNode/LED/commands", 1000, &LEDHandler::ledSubscribeCallback, this);
	ROS_DEBUG("LED handler created - /DeviceNode/LED/commands");
}

void LEDHandler::ledSubscribeCallback(const silbot3_msgs::Device_LED_Msg::ConstPtr& message) {
	string command = message->command;
	ROS_DEBUG("[LEDHandler] receive a message - %s", command.c_str());

	_ledServerProxy->on(message->id, message->bright, message->red, message->green, message->blue);
}
