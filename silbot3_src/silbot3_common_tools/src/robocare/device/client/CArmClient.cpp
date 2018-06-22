#include "CArmClient.h"

CArmClientProxy::CArmClientProxy(){

}

CArmClientProxy::~CArmClientProxy() {

}


CArm* CArmClientProxy::newInstance() {
	//cout << "[CArmClientProxy] : Create Sole Instance(Arm).." << endl;
	ROS_INFO("[CArmClientProxy] : Create Sole Instance(CArmROSStub)..");
	return new CArmROSStub();
}

CArmROSStub::CArmROSStub() {
	//this->_armClient = this->_nodeHandle.serviceClient<silbot3_msgs::ArmService>("/DeviceNode/ArmService",true);
	this->_armErrorClient = this->_nodeHandle.serviceClient<silbot3_msgs::Device_Error_Srv>("/DeviceNode/ArmErrorService",true);

	this->_armPublisher = this->_nodeHandle.advertise<silbot3_msgs::Device_Arm_Msg>("/DeviceNode/Arm/commands", 1000);
	while(this->_armPublisher.getNumSubscribers() < 1) {
		ROS_DEBUG("CArmRosStub : wating subscriber : %d", this->_armPublisher.getNumSubscribers());
		ros::Duration(0.1).sleep();
	}
	ROS_DEBUG("CArmRosStub : subscriber count  : %d", this->_armPublisher.getNumSubscribers());
	ROS_INFO("   - check finished..(OK) \n");
}



CArmROSStub::~CArmROSStub() {
}

int CArmROSStub::on( int armID ) {
	ROS_DEBUG("[CArmROSStub] on");
	/*
	silbot3_msgs::ArmService service;
	service.request.command = COMMAND_ARM_ON;
	service.request.motorID.push_back(armID);
	if(_armClient.call(service)) {
		//ROS_INFO("[CArmROSStub] COMMAND_ARM_ON success");
		return RETURN_SUCCESS;
	} else {
		//ROS_ERROR("[CArmROSStub] COMMAND_ARM_ON fail");
		return RETURN_FAIL;
	}
	*/
	silbot3_msgs::Device_Arm_Msg message;
	message.command = COMMAND_ARM_ON;
	message.motorID.push_back(armID);
	_armPublisher.publish(message);
	return RETURN_SUCCESS;
}

int CArmROSStub::off( int armID ) {
	ROS_DEBUG("[CArmROSStub] off");
	/*
	silbot3_msgs::ArmService service;
	service.request.command = COMMAND_ARM_OFF;
	service.request.motorID.push_back(armID);
	if(_armClient.call(service)) {
		//ROS_INFO("[CArmROSStub] COMMAND_ARM_OFF success");
		return RETURN_SUCCESS;
	} else {
		//ROS_ERROR("[CArmROSStub] COMMAND_ARM_OFF fail");
		return RETURN_FAIL;
	}
	*/
	silbot3_msgs::Device_Arm_Msg message;
	message.command = COMMAND_ARM_OFF;
	message.motorID.push_back(armID);
	_armPublisher.publish(message);
	return RETURN_SUCCESS;
}

int CArmROSStub::stop() {
	ROS_DEBUG("[CArmROSStub] stop");
	/*
	silbot3_msgs::ArmService service;
	service.request.command = COMMAND_ARM_STOP;

	if(_armClient.call(service)) {
		//ROS_INFO("[CArmROSStub] COMMAND_ARM_STOP success");
		return RETURN_SUCCESS;
	} else {
		//ROS_ERROR("[CArmROSStub] COMMAND_ARM_STOP fail");
		return RETURN_FAIL;
	}
	return 0;
	*/
	silbot3_msgs::Device_Arm_Msg message;
	message.command = COMMAND_ARM_STOP;
	_armPublisher.publish(message);
	return RETURN_SUCCESS;
}

int CArmROSStub::moveToHome( int armID ) {
	ROS_DEBUG("[CArmROSStub] moveToHome");
	/*
	silbot3_msgs::ArmService service;
	service.request.command = COMMAND_ARM_MOVE_TO_HOME;
	service.request.motorID.push_back(armID);

	if(_armClient.call(service)) {
		//ROS_INFO("[CArmROSStub] COMMAND_ARM_MOVE_TO_HOME success");
		return RETURN_SUCCESS;
	} else {
		//ROS_ERROR("[CArmROSStub] COMMAND_ARM_MOVE_TO_HOME fail");
		return RETURN_FAIL;
	}
	*/
	silbot3_msgs::Device_Arm_Msg message;
	message.command = COMMAND_ARM_MOVE_TO_HOME;
	message.motorID.push_back(armID);
	_armPublisher.publish(message);
	ros::spinOnce();
	return RETURN_SUCCESS;
}

int CArmROSStub::moveToPositionAll( int armID, double* angles, double speed ) {
	ROS_DEBUG("[CArmROSStub] moveToPositionAll");
	/*
	silbot3_msgs::ArmService service;
	service.request.command = COMMAND_ARM_MOVE_TO_POSITION_ALL;
	service.request.motorID.push_back(armID);
	service.request.angles.push_back(angles[0]);
	service.request.angles.push_back(angles[1]);
	service.request.angles.push_back(angles[2]);
	service.request.speeds.push_back(speed);
		
	if(_armClient.call(service)) {
		//ROS_INFO("[CArmROSStub] COMMAND_ARM_MOVE_TO_POSITION_ALL success");
		return RETURN_SUCCESS;
	} else {
		//ROS_ERROR("[CArmROSStub] COMMAND_ARM_MOVE_TO_POSITION_ALL fail");
		return RETURN_FAIL;
	}
	*/

	ROS_DEBUG("angles { %f, %f, %f}", angles[0], angles[1], angles[2]);

	silbot3_msgs::Device_Arm_Msg message;
	message.command = COMMAND_ARM_MOVE_TO_POSITION_ALL;
	message.motorID.push_back(armID);
	message.angles.push_back(angles[0]);
	message.angles.push_back(angles[1]);
	message.angles.push_back(angles[2]);
	message.speeds.push_back(speed);
	_armPublisher.publish(message);
	return RETURN_SUCCESS;
}

int CArmROSStub::moveToPositionWithAxisSpeed( int armID, double* angles, double* speeds ) {
	ROS_DEBUG("[CArmROSStub] moveToPositionWithAxisSpeed");
	/*
	silbot3_msgs::ArmService service;
	service.request.command = COMMAND_ARM_MOVE_TO_POSITION_WITH_AXIS_SPEED;
	service.request.motorID.push_back(armID);
	service.request.angles.push_back(angles[0]);
	service.request.angles.push_back(angles[1]);
	service.request.angles.push_back(angles[2]);
	service.request.speeds.push_back(speeds[0]);
	service.request.speeds.push_back(speeds[1]);
	service.request.speeds.push_back(speeds[2]);

	if(_armClient.call(service)) {
		//ROS_INFO("[CArmROSStub] COMMAND_ARM_MOVE_TO_POSITION_ALL success");
		return RETURN_SUCCESS;
	} else {
		//ROS_ERROR("[CArmROSStub] COMMAND_ARM_MOVE_TO_POSITION_ALL fail");
		return RETURN_FAIL;
	}
	*/
	silbot3_msgs::Device_Arm_Msg message;
	message.command = COMMAND_ARM_MOVE_TO_POSITION_WITH_AXIS_SPEED;
	message.motorID.push_back(armID);
	message.angles.push_back(angles[0]);
	message.angles.push_back(angles[1]);
	message.angles.push_back(angles[2]);
	message.speeds.push_back(speeds[0]);
	message.speeds.push_back(speeds[1]);
	message.speeds.push_back(speeds[2]);
	_armPublisher.publish(message);
	return RETURN_SUCCESS;
}

int CArmROSStub::moveToPositionAllWithPeriodic( int armID, double Angle, int stateType ) {
	ROS_DEBUG("[CArmROSStub] moveToPositionAllWithPeriodic");
	// if use... make it!
	return RETURN_FAIL;
}

void CArmROSStub::moveBothArmToPositionAll(double angles[6], int speed) {
	ROS_DEBUG("[CArmROSStub] moveBothArmToPositionAll");
	/*
	silbot3_msgs::ArmService service;
	service.request.command = COMMAND_ARM_MOVE_TO_POSITION_ALL_BOTH;
	//service.request.motorID.push_back(armID);
	service.request.angles.push_back(angles[0]);
	service.request.angles.push_back(angles[1]);
	service.request.angles.push_back(angles[2]);
	service.request.angles.push_back(angles[3]);
	service.request.angles.push_back(angles[4]);
	service.request.angles.push_back(angles[5]);
	service.request.speeds.push_back(speed);
	
	if(_armClient.call(service)) {
		//ROS_INFO("[CArmROSStub] COMMAND_ARM_MOVE_TO_POSITION_ALL success");
		//return RETURN_SUCCESS;
	} else {
		//ROS_ERROR("[CArmROSStub] COMMAND_ARM_MOVE_TO_POSITION_ALL fail");
		//return RETURN_FAIL;
	}
	*/
	silbot3_msgs::Device_Arm_Msg message;
	message.command = COMMAND_ARM_MOVE_TO_POSITION_ALL_BOTH;
	message.angles.push_back(angles[0]);
	message.angles.push_back(angles[1]);
	message.angles.push_back(angles[2]);
	message.angles.push_back(angles[3]);
	message.angles.push_back(angles[4]);
	message.angles.push_back(angles[5]);
	message.speeds.push_back(speed);
	_armPublisher.publish(message);
}

void CArmROSStub::moveBothArmToPositionWithAxisSpeed(double angles[6], int speeds[6]) {
	ROS_DEBUG("[CArmROSStub] moveBothArmToPositionWithAxisSpeed");
	silbot3_msgs::Device_Arm_Msg message;
	message.command = COMMAND_ARM_MOVE_TO_POSITION_ALL_BOTH_WITH_AXIS_SPEED;
	message.angles.push_back(angles[0]);
	message.angles.push_back(angles[1]);
	message.angles.push_back(angles[2]);
	message.angles.push_back(angles[3]);
	message.angles.push_back(angles[4]);
	message.angles.push_back(angles[5]);
	message.speeds.push_back(speeds[0]);
	message.speeds.push_back(speeds[1]);
	message.speeds.push_back(speeds[2]);
	message.speeds.push_back(speeds[3]);
	message.speeds.push_back(speeds[4]);
	message.speeds.push_back(speeds[5]);
	_armPublisher.publish(message);
}

void CArmROSStub::getArmPosition(int armId, double angle[6]) {
	ROS_DEBUG("[CArmROSStub] getArmPosition");
	// if use... make it!
	//return RETURN_FAIL;
}

int CArmROSStub::setPeriodic( int armID, int milisecondTime ) {
	ROS_DEBUG("[CArmROSStub] setPeriodic");
	// if use... make it!
	return RETURN_FAIL;
}

CArmData CArmROSStub::waitRunOnPeriodic( float index ) {
	ROS_DEBUG("[CArmROSStub] waitRunOnPeriodic");
	CArmData data;
	return data;
}

void CArmROSStub::emergencyStop() {
	ROS_DEBUG("[CArmROSStub] emergencyStop");
	silbot3_msgs::Device_Arm_Msg message;
	message.command = COMMAND_ARM_EMERGENCY_STOP;

	_armPublisher.publish(message);

}
void CArmROSStub::emergencyRelease() {
	ROS_DEBUG("[CArmROSStub] emergencyStop");
	silbot3_msgs::Device_Arm_Msg message;
	message.command = COMMAND_ARM_EMERGENCY_RELEASE;
	_armPublisher.publish(message);
}

void CArmROSStub::setArmHomePosition() {
	ROS_DEBUG("[CArmROSStub] setArmHomePosition");
	silbot3_msgs::Device_Arm_Msg message;
	message.command = COMMAND_ARM_SET_HOME_POSITION;
	_armPublisher.publish(message);
}

void CArmROSStub::detectingError(short errorCode[6]) {
	silbot3_msgs::Device_Error_Srv service;
	service.request.command = COMMAND_ARM_DETECTING_ERROR;
	if(_armErrorClient.call(service)) {
		//ROS_INFO("[CWheelROSStub] COMMAND_WHEEL_RESET success");
		for(int i=0; i<6; i++) {
			errorCode[i] = (short)service.response.iParams.at(i);
		}

	} else {
		//ROS_ERROR("[CWheelROSStub] COMMAND_WHEEL_RESET fail");
	}
}
