#include "ArmHandler.h"

#include <fstream>

#include "../../../reactive/erobot/ERobotCore.h"

using namespace cir::devicenode;


//ofstream SaveFile("C:\\opt\\devicenode_arm_handle.txt");
//int i;
ArmHandler::ArmHandler() {
	initHandler();
	_armServerProxy = new CArmERobot();
	this->_ARM_THREAD_FLAG = false;
//	this->_this_thread = boost::thread(boost::bind(&ArmHandler::armPublishThread, this));
//	i = 0;
}

ArmHandler::~ArmHandler() {

}

void ArmHandler::initHandler() {
	//this->_armServiceServer = this->_nodeHandle.advertiseService("/DeviceNode/ArmService", &ArmHandler::armServiceCallback, this);
	this->_armErrorServiceServer = this->_nodeHandle.advertiseService("/DeviceNode/ArmErrorService", &ArmHandler::armErrorServiceCallback, this);
	this->_armDataServiceServer = this->_nodeHandle.advertiseService("/DeviceNode/ArmDataService", &ArmHandler::armDataServiceCallback, this);
	this->_armSubscriber = this->_nodeHandle.subscribe<silbot3_msgs::Device_Arm_Msg>("/DeviceNode/Arm/commands", 1000, &ArmHandler::armSubscribeCallback, this);
//	this->_armPublisher = this->_nodeHandle.advertise<silbot3_msgs::Device_Arm_Msg>("/DeviceNode/Arm/data", 1000);
	ROS_DEBUG("\n\nArm handler created - /DeviceNode/Arm/commands\n\n");
//	ROS_DEBUG("Arm handler created - /DeviceNode/Arm/data");
}

void ArmHandler::armPublishThread() {

	this->_ARM_THREAD_FLAG = true;
	ros::Rate loop_rate(10);

	double rightAngle[3];
	double leftAngle[3];

	while( ros::ok() && this->_ARM_THREAD_FLAG ) {

		silbot3_msgs::Device_Arm_Msg message;

		_armServerProxy->getArmPosition(0, rightAngle);
		_armServerProxy->getArmPosition(1, leftAngle);

		message.command = COMMAND_ARM_GET_POSITION;
		message.angles.push_back(rightAngle[0]);
		message.angles.push_back(rightAngle[1]);
		message.angles.push_back(rightAngle[2]);
		message.angles.push_back(leftAngle[0]);
		message.angles.push_back(leftAngle[1]);
		message.angles.push_back(leftAngle[2]);

		this->_armPublisher.publish(message);

		ros::spinOnce();
		loop_rate.sleep();
	}

}

bool ArmHandler::armDataServiceCallback(silbot3_msgs::Device_Data_Srv::Request &request, silbot3_msgs::Device_Data_Srv::Response &response) {

	string command = request.command;
//	ROS_INFO("[WheelHandler] wheelServiceCallback receive a message - %s", command);

	double angle[6] = {0,0,0,0,0,0};
	_armServerProxy->getArmPosition(2, angle);
	for(int i=0; i<6; i++) {
		response.dParams.push_back(angle[i]);
	}

	return RETURN_SUCCESS;

}


void ArmHandler::armSubscribeCallback(const silbot3_msgs::Device_Arm_Msg::ConstPtr& message) {
	//SaveFile << i++ << endl;
	string command = message->command;
	ROS_DEBUG("[ArmHandler] receive a message - %s", command.c_str());

	if(command == COMMAND_ARM_ON) {
		_armServerProxy->on(message->motorID.at(0));
	} else if(command == COMMAND_ARM_OFF) {
		_armServerProxy->off(message->motorID.at(0));
	} else if(command == COMMAND_ARM_STOP) {
		_armServerProxy->stop();
	} else if(command == COMMAND_ARM_MOVE_TO_HOME) {
		_armServerProxy->moveToHome(message->motorID.at(0));
	} else if(command == COMMAND_ARM_MOVE_TO_POSITION_ALL) {
	ROS_DEBUG("[ArmHandler] armId  : %d, angle1 : %d, angle2 : %d, angle3: %d, speed : %d", message->motorID[0],  message->angles[0], message->angles[1], message->angles[2], message->speeds[0]);
		double angles[3] = {
			(double)message->angles.at(0),
			(double)message->angles.at(1),
			(double)message->angles.at(2)	};
		double speed = message->speeds.at(0);
		_armServerProxy->moveToPositionAll( message->motorID.at(0), angles , speed );


	} else if(command == COMMAND_ARM_MOVE_TO_POSITION_WITH_AXIS_SPEED) {
		double angles[3] = {
			(double)message->angles.at(0),
			(double)message->angles.at(1),
			(double)message->angles.at(2)	};
		double speeds[3] = {
			(double)message->speeds.at(0),
			(double)message->speeds.at(1),
			(double)message->speeds.at(2)	};
		_armServerProxy->moveToPositionWithAxisSpeed(message->motorID.at(0), angles, speeds);

	} else if(command == COMMAND_ARM_MOVE_TO_POSITION_ALL_BOTH_WITH_AXIS_SPEED) {
		double angles[6] = {
				(double)message->angles.at(0),
				(double)message->angles.at(1),
				(double)message->angles.at(2),
				(double)message->angles.at(3),
				(double)message->angles.at(4),
				(double)message->angles.at(5)	};
		int speeds[6] = {
				(int)message->speeds.at(0),
				(int)message->speeds.at(1),
				(int)message->speeds.at(2),
				(int)message->speeds.at(3),
				(int)message->speeds.at(4),
				(int)message->speeds.at(5)	};
		_armServerProxy->moveBothArmToPositionWithAxisSpeed(angles, speeds);
	}
	else if(command == COMMAND_ARM_MOVE_TO_POSITION_ALL_BOTH) {
		ROS_DEBUG("[ArmHandler] R_angle1 : %d, R_angle2 : %d, R_angle3: %d, L_angle1 : %d, L_angle2 : %d, L_angle3: %d,speed : %d",
					message->angles[0], message->angles[1], message->angles[2], message->angles[3], message->angles[4], message->angles[5],message->speeds[0]);
		double angles[6] = {
			(double)message->angles.at(0),
			(double)message->angles.at(1),
			(double)message->angles.at(2),
			(double)message->angles.at(3),
			(double)message->angles.at(4),
			(double)message->angles.at(5)	};
		double speed = message->speeds.at(0);
		_armServerProxy->moveBothArmToPositionAll(angles, speed);

	} else if(command == COMMAND_ARM_GET_POSITION) {
//		double angles[3];
//		_armServerProxy->getArmPosition(message->motorID.at(0), angles);
	} else if(command == COMMAND_ARM_EMERGENCY_STOP) {
		_armServerProxy->emergencyStop();
	} else if(command == COMMAND_ARM_EMERGENCY_RELEASE) {
		_armServerProxy->emergencyRelease();
	} else if(command == COMMAND_ARM_SET_HOME_POSITION) {
		_armServerProxy->setArmHomePosition();
	}
//#define COMMAND_ARM_RELEASE_TORQUE						"ARM_RELEASE_TORQUE"
//#define COMMAND_ARM_START_TEACHING						"ARM_START_TEACHING"
//#define COMMAND_ARM_STOP_TEACHING						"ARM_STOP_TEACHING"

	else if(command == COMMAND_ARM_RELEASE_TORQUE) {

		ERobotCore::getInstance()->releaseTorue();
	} else if(command == COMMAND_ARM_START_TEACHING) {
		ERobotCore::getInstance()->startArmsTeaching();
	} else if(command == COMMAND_ARM_STOP_TEACHING) {
		ERobotCore::getInstance()->stopArmsTeaching();
	}
}

bool ArmHandler::armServiceCallback(silbot3_msgs::Device_Arm_Srv::Request &request, silbot3_msgs::Device_Arm_Srv::Response &response) {

	string command = request.command;

	if(command == COMMAND_ARM_ON) {
		_armServerProxy->on(request.motorID.at(0));

	} else if(command == COMMAND_ARM_OFF) {
		_armServerProxy->off(request.motorID.at(0));

	} else if(command == COMMAND_ARM_STOP) {
		_armServerProxy->stop();

	} else if(command == COMMAND_ARM_MOVE_TO_HOME) {
		_armServerProxy->moveToHome(request.motorID.at(0));

	} else if(command == COMMAND_ARM_MOVE_TO_POSITION_ALL) {
		double angles[3] = {	request.angles.at(0),
			(double)request.angles.at(1),
			(double)request.angles.at(2)	};
		double speed = request.speeds.at(0);
		_armServerProxy->moveToPositionAll( request.motorID.at(0), angles , speed );

	} else if(command == COMMAND_ARM_MOVE_TO_POSITION_WITH_AXIS_SPEED) {
		double angles[3] = {	request.angles.at(0),
			(double)request.angles.at(1),
			(double)request.angles.at(2)	};
		double speeds[3] = {	request.speeds.at(0),
			(double)request.speeds.at(1),
			(double)request.speeds.at(2)	};
		_armServerProxy->moveToPositionWithAxisSpeed(request.motorID.at(0), angles, speeds);

	} else if(command == COMMAND_ARM_MOVE_TO_POSITION_ALL_BOTH) {
		double angles[6] = {	request.angles.at(0),
			(double)request.angles.at(1),
			(double)request.angles.at(2),
			(double)request.angles.at(3),
			(double)request.angles.at(4),
			(double)request.angles.at(5)	};
		double speed = request.speeds.at(0);
		_armServerProxy->moveBothArmToPositionAll(angles, speed);

	} else if(command == COMMAND_ARM_GET_POSITION) {
		double angles[3];
		_armServerProxy->getArmPosition(request.motorID.at(0), angles);
	}

	response.flag = true;

	return true;
}

bool ArmHandler::armErrorServiceCallback(silbot3_msgs::Device_Error_Srv::Request &request, silbot3_msgs::Device_Error_Srv::Response &response) {

	string command = request.command;
//	ROS_INFO("[WheelHandler] wheelServiceCallback receive a message - %s", command);

	short errorCode[6] = {0,};
	_armServerProxy->detectingError(errorCode);
	for(int i=0; i<6; i++) {
		response.iParams.push_back(errorCode[i]);
	}
	return RETURN_SUCCESS;
}
