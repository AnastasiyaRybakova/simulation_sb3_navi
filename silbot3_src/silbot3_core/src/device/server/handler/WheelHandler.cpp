#include "WheelHandler.h"
#include <cmath>

using namespace cir::devicenode;

WheelHandler::WheelHandler() {
	initHandler();
	_wheelServerProxy = new CWheelERobot();
}

WheelHandler::~WheelHandler() {

}

void WheelHandler::initHandler() {
	this->_wheelServiceServer = this->_nodeHandle.advertiseService("/DeviceNode/WheelService", &WheelHandler::wheelServiceCallback, this);
	this->_wheelErrorServiceServer = this->_nodeHandle.advertiseService("/DeviceNode/WheelErrorService", &WheelHandler::wheelErrorServiceCallback, this);
	this->_subscriber = this->_nodeHandle.subscribe<silbot3_msgs::Device_Wheel_Msg>("/DeviceNode/Wheel/commands", 1000, &WheelHandler::wheelSubscribeCallBack, this);
	this->_twistSubscriber = this->_nodeHandle.subscribe<geometry_msgs::Twist>("/cmd_vel", 1000, &WheelHandler::twistMsgCallback, this);
	ROS_INFO("Wheel handler created - /DeviceNode/Wheel/commands");
}

void WheelHandler::twistMsgCallback(const geometry_msgs::Twist::ConstPtr& msg) {
	double x = msg->linear.x; // meter / sec
	double y = msg->linear.y; // meter / sec
	double z = msg->angular.z; // radian

	double deg = z * 180 / M_PI;

	if( x == 0.0 && y == 0.0 && z == 0.0) {
		_wheelServerProxy->stop();
	} else {
		_wheelServerProxy->moveByVelocityXYT(x * 1000., y * 1000., -deg);
	}
}

void WheelHandler::wheelSubscribeCallBack(const silbot3_msgs::Device_Wheel_Msg::ConstPtr& message) {

	string command = message->command;
	ROS_DEBUG("[WheelHandler] receive a message - %s", command.c_str());

	if(command == COMMAND_WHEEL_ON) {
		_wheelServerProxy->on();
	} else if(command == COMMAND_WHEEL_OFF) {
		_wheelServerProxy->off();
	} else if(command == COMMAND_WHEEL_STOP) {
		_wheelServerProxy->stop();
	} else if(command == COMMAND_WHEEL_MOVE_BY_TR_VELOCITY) {
		_wheelServerProxy->moveByTRVelocity(
			message->dParams.at(0),
			message->dParams.at(1)
			);
	} else if(command == COMMAND_WHEEL_MOVE_BY_WHEEL_VELOCITY) {
		int size = message->dParams.size();
		if(size == 2) {
			_wheelServerProxy->moveByWheelVelocity(
				message->dParams.at(0),
				message->dParams.at(1)
				);
		} else if(size == 3) {
			_wheelServerProxy->moveByWheelVelocity(
				message->dParams.at(0),
				message->dParams.at(1),
				message->dParams.at(2)
				);
		}
	}else if(command == COMMAND_WHEEL_MOVE_TO_DELTA_X) {
		_wheelServerProxy->moveToDeltaX(
			message->iParams.at(0),
			message->dParams.at(0)
			);
	} else if(command == COMMAND_WHEEL_MOVE_TO_DELTA_THETA) {
		_wheelServerProxy->moveToDeltaTheta(
			message->iParams.at(0),
			message->iParams.at(1),
			message->dParams.at(0)
			);
	} else if(command == COMMAND_WHEEL_MOVE_TO_DELTA_XYTHETA) {
		_wheelServerProxy->moveToDeltaXYTheta(
			message->iParams.at(0),
			message->iParams.at(1),
			message->iParams.at(2),
			message->dParams.at(0)
			);
	} else if(command == COMMAND_WHEEL_MOVE_BY_VELOCITY_XYT) {
		_wheelServerProxy->moveByVelocityXYT(
			message->dParams.at(0),
			message->dParams.at(1),
			message->dParams.at(2)
			);
	} else if(command == COMMAND_WHEEL_RESET) {
		_wheelServerProxy->reset();
	} else if(command == COMMAND_WHEEL_EMERGENCY_STOP) {
		_wheelServerProxy->emergencyStop();
	} else if(command == COMMAND_WHEEL_EMERGENCY_RELEASE) {
		_wheelServerProxy->emergencyRelease();
	} else if(command == COMMAND_WHEEL_EMERGENCY_MOVE_BY_VELOCITY_XYT) {
		_wheelServerProxy->emergencyMoveByVelocityXYT(
				message->dParams.at(0),
				message->dParams.at(1),
				message->dParams.at(2)
			);
	} else if(command == COMMAND_WHEEL_START_AUTO_CHARGE) {
		_wheelServerProxy->startAutoCharge();
	} else if(command == COMMAND_WHEEL_STOP_AUTO_CHARGE) {
		_wheelServerProxy->stopAutoCharge();
	}


	else {
		cout << "[WheelHandler] unrecognized command. " << endl;
	}

}

bool WheelHandler::wheelServiceCallback(silbot3_msgs::Device_Wheel_Srv::Request &request, silbot3_msgs::Device_Wheel_Srv::Response &response) {

	string command = request.command;
	ROS_INFO("[WheelHandler] wheelServiceCallback receive a message - %s", command.c_str());

	if(command == COMMAND_WHEEL_ON) {
		_wheelServerProxy->on();
		return true;
	} else if(command == COMMAND_WHEEL_OFF) {
		_wheelServerProxy->off();
		return true;
	} else if(command == COMMAND_WHEEL_STOP) {
		_wheelServerProxy->stop();
		return true;
	} else if(command == COMMAND_WHEEL_MOVE_BY_TR_VELOCITY) {
		_wheelServerProxy->moveByTRVelocity(
			request.dParams.at(0),
			request.dParams.at(1)
			);
		return true;
	} else if(command == COMMAND_WHEEL_MOVE_BY_WHEEL_VELOCITY) {
		int size = request.dParams.size();
		if(size == 2) {
			_wheelServerProxy->moveByWheelVelocity(
				request.dParams.at(0),
				request.dParams.at(1)
				);
		} else if(size == 3) {
			_wheelServerProxy->moveByWheelVelocity(
				request.dParams.at(0),
				request.dParams.at(1),
				request.dParams.at(2)
				);
		}
		return true;
	} else if(command == COMMAND_WHEEL_MOVE_TO_DELTA_X) {
		_wheelServerProxy->moveToDeltaX(
			request.iParams.at(0),
			request.dParams.at(0)
			);
		return true;
	} else if(command == COMMAND_WHEEL_MOVE_TO_DELTA_THETA) {
		_wheelServerProxy->moveToDeltaTheta(
			request.iParams.at(0),
			request.iParams.at(1),
			request.dParams.at(0)
			);
		return true;
	} else if(command == COMMAND_WHEEL_MOVE_TO_DELTA_XYTHETA) {
		_wheelServerProxy->moveToDeltaXYTheta(
			request.iParams.at(0),
			request.iParams.at(1),
			request.iParams.at(2),
			request.dParams.at(0)
			);
		return true;
	} else if(command == COMMAND_WHEEL_MOVE_BY_VELOCITY_XYT) {
		_wheelServerProxy->moveByVelocityXYT(
			request.dParams.at(0),
			request.dParams.at(1),
			request.dParams.at(2)
			);
		return true;
	} else if(command == COMMAND_WHEEL_EMERGENCY_STOP) {
		_wheelServerProxy->emergencyStop();
		return true;
	} else if(command == COMMAND_WHEEL_RESET) {
		_wheelServerProxy->reset();
		return true;
	} else {
		cout << "[WheelHandler] unrecognized command. " << endl;
	}
	return false;
}

bool WheelHandler::wheelErrorServiceCallback(silbot3_msgs::Device_Error_Srv::Request &request, silbot3_msgs::Device_Error_Srv::Response &response) {

	string command = request.command;
//	ROS_INFO("[WheelHandler] wheelServiceCallback receive a message - %s", command);

	short errorCode[9] = {0,};
	_wheelServerProxy->selfDiagnosis(errorCode);
	for(int i=0; i<9; i++) {
		response.iParams.push_back(errorCode[i]);
	}
	return RETURN_SUCCESS;
}
