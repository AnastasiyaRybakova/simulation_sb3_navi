#include "PantiltHandler.h"
#include <ros/ros.h>
#include <vector>

using namespace cir::devicenode;
using namespace std;

PantiltHandler::PantiltHandler() {
	initHandler();
	_pantiltServerProxy = new CPanTiltERobot();
	this->_PANTILT_THREAD_FLAG = false;
//	this->_this_thread = boost::thread(boost::bind(&PantiltHandler::pantiltPublishThread, this));
}

PantiltHandler::~PantiltHandler() {

}

void PantiltHandler::initHandler() {
	//this->_pantiltServiceServer = this->_nodeHandle.advertiseService("/DeviceNode/PantiltService", &PantiltHandler::pantiltServiceCallback, this);
	this->_pantiltErrorServiceServer = this->_nodeHandle.advertiseService("/DeviceNode/PantiltErrorService", &PantiltHandler::pantiltErrorServiceCallback, this);
	this->_pantiltDataServiceServer = this->_nodeHandle.advertiseService("/DeviceNode/PantiltDataService", &PantiltHandler::pantiltDataServiceCallback, this);
	this->_pantiltSubscriber = this->_nodeHandle.subscribe<silbot3_msgs::Device_ErobotPantilt_Msg>("/DeviceNode/Pantilt/commands", 1000, &PantiltHandler::pantiltSubscriberCallback, this);
	this->_twistMsgSubscriber = this->_nodeHandle.subscribe<geometry_msgs::Twist>("/pantilt/cmd_vel", 1000, &PantiltHandler::twistMsgCallback, this);
//	this->_pantiltPublisher = this->_nodeHandle.advertise<silbot3_msgs::Device_ErobotPantilt_Msg>("/DeviceNode/Pantilt/data", 1000);
	ROS_INFO("Pantilt handler created - /DeviceNode/Pantilt/commands");
}

void PantiltHandler::pantiltPublishThread() {

	this->_PANTILT_THREAD_FLAG = true;
	ros::Rate loop_rate(10);

	CPanTiltPosition positon;

	while( ros::ok() && this->_PANTILT_THREAD_FLAG ) {

		silbot3_msgs::Device_ErobotPantilt_Msg message;

		_pantiltServerProxy->getPosition(positon);


		message.command = COMMAND_PANTILT_GETPOSITION;
		message.angles.push_back(positon.getDegreeLeftRight());
		message.angles.push_back(positon.getDegreeUpDown());
		message.angles.push_back(positon.getXtionTilt());

		this->_pantiltPublisher.publish(message);

		ros::spinOnce();
		loop_rate.sleep();
	}

}

void PantiltHandler::twistMsgCallback(const geometry_msgs::Twist::ConstPtr& message) {
	//ROS_DEBUG("PantiltHandler::twistMsgCallback");

	float panvel = message->linear.x;
	float tiltvel = message->linear.y;

	float panang = message->angular.x;
	float tiltang = message->angular.y;

	_pantiltServerProxy->moveToAbsolutePosition(panang, tiltang, panvel);

}


void PantiltHandler::pantiltSubscriberCallback(const silbot3_msgs::Device_ErobotPantilt_Msg::ConstPtr& message) {
	string command = message->command;
	ROS_DEBUG("[PantiltHandler] receive a message - %s", command.c_str());

	if(command == COMMAND_PANTILT_ON) {
		_pantiltServerProxy->on();
	} else if(command == COMMAND_PANTILT_OFF) {
		_pantiltServerProxy->off();
	} else if(command == COMMAND_PANTILT_STOP) {
		_pantiltServerProxy->stop();
	} else if(command == COMMAND_PANTILT_MOVE_TO_HOME) {
		_pantiltServerProxy->moveToHome(
			message->speeds.at(0)
			);
	} else if(command == COMMAND_PANTILT_MOVE_TO_RELATIVE_POSITION) {
		_pantiltServerProxy->moveToRelativePosition(
			message->angles.at(0),
			message->angles.at(1),
			message->speeds.at(0)
			);
	} else if(command == COMMAND_PANTILT_MOVE2MOTOR_SYNC) {
		_pantiltServerProxy->move2MotorSync(
			message->motorID.at(0),
			message->speeds.at(0),
			message->angles.at(0),
			message->motorID.at(1),
			message->speeds.at(1),
			message->angles.at(1)
			);
	} else if(command == COMMAND_PANTILT_MOVE_MOTOR) {
		_pantiltServerProxy->moveMotor(
			message->motorID.at(0),
			message->speeds.at(0),
			message->angles.at(0)
			);
	} else if(command == COMMAND_PANTILT_MOVE_ABSOLUTE_POSITION) {
		_pantiltServerProxy->moveToAbsolutePosition(
			message->angles.at(0),
			message->angles.at(1),
			message->speeds.at(0)
			);

	} else if(command == COMMAND_PANTILT_MOVE_ABSOLUTE_POSITION_WITH_SPEED) {
		_pantiltServerProxy->moveToAbsolutePositionWithSpeed(
			message->angles.at(0),
			message->angles.at(1),
			message->speeds.at(0),
			message->speeds.at(1)
			);

	} else if(command == COMMAND_PANTILT_MOVE_ABSOLUTE_POSITION_ALL) {

		// ROS_DEBUG("\n\npantilt COMMAND_PANTILT_MOVE_ABSOLUTE_POSITION_ALL value printing\n");
		// ROS_DEBUG("pantilt : angle printing ...");
		// for(int i = 0; i < message->angles.size(); i++) {
		// 	ROS_DEBUG("%f", message->angles[i]);
		// }
		//
		// ROS_DEBUG("pantilt : sppeds printing ...");
		// for(int i = 0; i < message->speeds.size(); i++) {
		// 	ROS_DEBUG("%f", message->speeds[i]);
		// }
		//
		// ROS_DEBUG("\n----------------------------\n");

		_pantiltServerProxy->moveToABSAll(
			message->angles.at(0),
			message->angles.at(1),
			message->angles.at(2),
			message->speeds.at(0),
			message->speeds.at(1),
			message->speeds.at(2)
			);

	} else if(command == COMMAND_PANTILT_GETPOSITION) {
//		CPanTiltPosition returnData;
//		_pantiltServerProxy->getPosition(returnData);

	} else if(command == COMMAND_PANTILT_SET_COMPLIANCE) {
		_pantiltServerProxy->setCompliance(message->speeds.at(0));

	} else if(command == COMMAND_PANTILT_MOVE_XTION_TILT) {
		_pantiltServerProxy->moveXtionTilt(
				message->angles.at(0),
				message->speeds.at(0)
			);

	} else if(command == COMMAND_PANTILT_EMERGENCY_STOP) {
		_pantiltServerProxy->emergencyStop();
	} else if(command == COMMAND_PANTILT_EMERGENCY_RELEASE) {
		_pantiltServerProxy->emergencyRelease();
	}
	else if(command == COMMAND_PANTILT_RELEASE_TORQUE) {
//		cout << "handler :: COMMAND_PANTILT_RELEASE_TORQUE" << endl;
		ERobotCore::getInstance()->releaseTorue();
	} else if(command == COMMAND_PANTILT_START_TEACHING) {
//		cout << "handler :: COMMAND_PANTILT_START_TEACHING" << endl;
		ERobotCore::getInstance()->startHeadTeaching();
	} else if(command == COMMAND_PANTILT_STOP_TEACHING) {
//		cout << "handler :: COMMAND_PANTILT_STOP_TEACHING" << endl;
		ERobotCore::getInstance()->stopHeadTeaching();
	}
	else {
		cout << "[PantiltHandler] unrecognized command. " << endl;
	}
}

bool PantiltHandler::pantiltServiceCallback(silbot3_msgs::Device_Pantilt_Srv::Request &request, silbot3_msgs::Device_Pantilt_Srv::Response &response) {

	string command = request.command;
	if(command == COMMAND_PANTILT_ON) {
		_pantiltServerProxy->on();
		return true;
	} else if(command == COMMAND_PANTILT_OFF) {
		_pantiltServerProxy->off();
		return true;
	} else if(command == COMMAND_PANTILT_STOP) {
		_pantiltServerProxy->stop();
		return true;
	} else if(command == COMMAND_PANTILT_MOVE_TO_HOME) {
		_pantiltServerProxy->moveToHome(
			request.speeds.at(0)
			);
		return true;
	} else if(command == COMMAND_PANTILT_MOVE_TO_RELATIVE_POSITION) {
		_pantiltServerProxy->moveToRelativePosition(
			request.angles.at(0),
			request.angles.at(1),
			request.speeds.at(0)
			);
		return true;
	} else if(command == COMMAND_PANTILT_MOVE2MOTOR_SYNC) {
		_pantiltServerProxy->move2MotorSync(
			request.motorID.at(0),
			request.speeds.at(0),
			request.angles.at(0),
			request.motorID.at(1),
			request.speeds.at(1),
			request.angles.at(1)
			);
		return true;
	} else if(command == COMMAND_PANTILT_MOVE_MOTOR) {
		_pantiltServerProxy->moveMotor(
			request.motorID.at(0),
			request.speeds.at(0),
			request.angles.at(0)
			);
		return true;
	} else if(command == COMMAND_PANTILT_MOVE_ABSOLUTE_POSITION) {
		_pantiltServerProxy->moveToAbsolutePosition(
			request.angles.at(0),
			request.angles.at(1),
			request.speeds.at(0)
			);
		return true;
	} else if(command == COMMAND_PANTILT_MOVE_ABSOLUTE_POSITION_ALL) {
		_pantiltServerProxy->moveToABSAll(
			request.angles.at(0),
			request.angles.at(1),
			request.angles.at(2),
			request.speeds.at(0),
			request.speeds.at(1),
			request.speeds.at(2)
			);
		return true;
	} else if(command == COMMAND_PANTILT_GETPOSITION) {
		CPanTiltPosition returnData;
		_pantiltServerProxy->getPosition(returnData);
		// TODO
		return true;
	} else if(command == COMMAND_PANTILT_SET_COMPLIANCE) {
		_pantiltServerProxy->setCompliance(request.speeds.at(0));
		return true;
	}  else {
		cout << "[GyroHandler] unrecognized command. " << endl;
	}
	return false;
}


bool PantiltHandler::pantiltErrorServiceCallback(silbot3_msgs::Device_Error_Srv::Request &request, silbot3_msgs::Device_Error_Srv::Response &response) {

	string command = request.command;
//	ROS_INFO("[WheelHandler] wheelServiceCallback receive a message - %s", command);

	short errorCode[3] = {0,};
	_pantiltServerProxy->detectingError(errorCode);
	for(int i=0; i<3; i++) {
		response.iParams.push_back(errorCode[i]);
	}
	return RETURN_SUCCESS;
}

bool PantiltHandler::pantiltDataServiceCallback(silbot3_msgs::Device_Data_Srv::Request &request, silbot3_msgs::Device_Data_Srv::Response &response) {

	string command = request.command;
//	ROS_INFO("[WheelHandler] wheelServiceCallback receive a message - %s", command);

	CPanTiltPosition position;
	_pantiltServerProxy->getPosition(position);

	response.dParams.push_back(position.getDegreeLeftRight());
	response.dParams.push_back(position.getDegreeUpDown());
	response.dParams.push_back(position.getXtionTilt());

	return RETURN_SUCCESS;
}
