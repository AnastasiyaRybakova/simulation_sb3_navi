#include "CPanTiltClient.h"

CPanTiltClientProxy::CPanTiltClientProxy()
{
}

CPanTiltClientProxy::~CPanTiltClientProxy()
{
}

CPanTilt* CPanTiltClientProxy::newInstance(){
	//cout << "[CPanTiltClientProxy] : Create Sole Instance(Pantilt).." << endl;
	ROS_INFO("[CPanTiltClientProxy] : Create Sole Instance(CPanTiltROSStub)..");
	return new CPanTiltROSStub();
}

CPanTiltROSStub::CPanTiltROSStub() {
	this->_pantiltErrorClient = this->_nodeHandle.serviceClient<silbot3_msgs::Device_Error_Srv>("/DeviceNode/PantiltErrorService",true);
	this->_pantiltPositionDataClient = this->_nodeHandle.serviceClient<silbot3_msgs::Device_Data_Srv>("/DeviceNode/PantiltDataService",true);

	this->_pantiltPublisher = this->_nodeHandle.advertise<silbot3_msgs::Device_ErobotPantilt_Msg>("/DeviceNode/Pantilt/commands", 1000);
	ROS_DEBUG("CPanTiltROSStub publisher created");

	while(this->_pantiltPublisher.getNumSubscribers() < 1) {
		ros::Duration(0.5).sleep();
		ros::spinOnce();
		ROS_DEBUG("CPanTiltROSStub wait for subscriber");
	}

}

CPanTiltROSStub::~CPanTiltROSStub()
{
}

void CPanTiltROSStub::close()
{
}

int CPanTiltROSStub::on() {
	/*
	silbot3_msgs::PantiltService service;
	service.request.command = COMMAND_PANTILT_ON;
	if(_pantiltClient.call(service)) {
		//ROS_INFO("[CPanTiltROSStub] COMMAND_PANTILT_ON success");
		return RETURN_SUCCESS;
	} else {
		//ROS_ERROR("[CPanTiltROSStub] COMMAND_PANTILT_ON fail");
		return RETURN_FAIL;
	}
	*/
	silbot3_msgs::Device_ErobotPantilt_Msg message;
	message.command = COMMAND_PANTILT_ON;
	_pantiltPublisher.publish(message);
	return RETURN_SUCCESS;
}


int CPanTiltROSStub::off() {
	/*
	silbot3_msgs::PantiltService service;
	service.request.command = COMMAND_PANTILT_OFF;
	if(_pantiltClient.call(service)) {
		//ROS_INFO("[CPanTiltROSStub] COMMAND_PANTILT_OFF success");
		return RETURN_SUCCESS;
	} else {
		//ROS_ERROR("[CPanTiltROSStub] COMMAND_PANTILT_OFF fail");
		return RETURN_FAIL;
	}
	*/
	silbot3_msgs::Device_ErobotPantilt_Msg message;
	message.command = COMMAND_PANTILT_OFF;
	_pantiltPublisher.publish(message);
	return RETURN_SUCCESS;
}


int CPanTiltROSStub::stop() {
	/*
	silbot3_msgs::PantiltService service;
	service.request.command = COMMAND_PANTILT_STOP;
	if(_pantiltClient.call(service)) {
		//ROS_INFO("[CPanTiltROSStub] COMMAND_PANTILT_STOP success");
		return RETURN_SUCCESS;
	} else {
		//ROS_ERROR("[CPanTiltROSStub] COMMAND_PANTILT_STOP fail");
		return RETURN_FAIL;
	}
	*/
	silbot3_msgs::Device_ErobotPantilt_Msg message;
	message.command = COMMAND_PANTILT_STOP;
	_pantiltPublisher.publish(message);
	return RETURN_SUCCESS;
}


int CPanTiltROSStub::moveToHome( int speed ) {
	/*
	silbot3_msgs::PantiltService service;
	service.request.command = COMMAND_PANTILT_MOVE_TO_HOME;
	service.request.speeds.push_back(speed);
	if(_pantiltClient.call(service)) {
		//ROS_INFO("[CPanTiltROSStub] COMMAND_PANTILT_MOVE_TO_HOME success");
		return RETURN_SUCCESS;
	} else {
		//ROS_ERROR("[CPanTiltROSStub] COMMAND_PANTILT_MOVE_TO_HOME fail");
		return RETURN_FAIL;
	}
	*/
	silbot3_msgs::Device_ErobotPantilt_Msg message;
	message.command = COMMAND_PANTILT_MOVE_TO_HOME;
	message.speeds.push_back(speed);
	_pantiltPublisher.publish(message);
	return RETURN_SUCCESS;
}


int CPanTiltROSStub::moveToAbsolutePosition( double degreeLeftRight, double degreeUpDown, int speed ) {
	/*
	silbot3_msgs::PantiltService service;
	service.request.command = COMMAND_PANTILT_MOVE_ABSOLUTE_POSITION;
	service.request.angles.push_back(degreeLeftRight);
	service.request.angles.push_back(degreeUpDown);
	service.request.speeds.push_back(speed);
	if(_pantiltClient.call(service)) {
		//ROS_INFO("[CPanTiltROSStub] COMMAND_PANTILT_MOVE_ABSOLUTE_POSITION success");
		return RETURN_SUCCESS;
	} else {
		//ROS_ERROR("[CPanTiltROSStub] COMMAND_PANTILT_MOVE_ABSOLUTE_POSITION fail");
		return RETURN_FAIL;
	}
	*/
	silbot3_msgs::Device_ErobotPantilt_Msg message;
	message.command = COMMAND_PANTILT_MOVE_ABSOLUTE_POSITION;
	message.angles.push_back(degreeLeftRight);
	message.angles.push_back(degreeUpDown);
	message.speeds.push_back(speed);
	_pantiltPublisher.publish(message);
	return RETURN_SUCCESS;
}

int CPanTiltROSStub::moveToAbsolutePositionWithSpeed(double degreeLeftRight, double degreeUpDown, int speedLeftRight, int speedUpDown)
{
	silbot3_msgs::Device_ErobotPantilt_Msg message;
	message.command = COMMAND_PANTILT_MOVE_ABSOLUTE_POSITION_WITH_SPEED;
	message.angles.push_back(degreeLeftRight);
	message.angles.push_back(degreeUpDown);
	message.speeds.push_back(speedLeftRight);
	message.speeds.push_back(speedUpDown);
	_pantiltPublisher.publish(message);
	return RETURN_SUCCESS;
}



int CPanTiltROSStub::moveToRelativePosition( double degreeLeftRight, double degreeUpDown, int speed ) {
	/*
	silbot3_msgs::PantiltService service;
	service.request.command = COMMAND_PANTILT_MOVE_TO_RELATIVE_POSITION;
	service.request.angles.push_back(degreeLeftRight);
	service.request.angles.push_back(degreeUpDown);
	service.request.speeds.push_back(speed);
	if(_pantiltClient.call(service)) {
		//ROS_INFO("[CPanTiltROSStub] COMMAND_PANTILT_MOVE_TO_RELATIVE_POSITION success");
		return RETURN_SUCCESS;
	} else {
		//ROS_ERROR("[CPanTiltROSStub] COMMAND_PANTILT_MOVE_TO_RELATIVE_POSITION fail");
		return RETURN_FAIL;
	}
	*/
	silbot3_msgs::Device_ErobotPantilt_Msg message;
	message.command = COMMAND_PANTILT_MOVE_TO_RELATIVE_POSITION;
	message.angles.push_back(degreeLeftRight);
	message.angles.push_back(degreeUpDown);
	message.speeds.push_back(speed);
	_pantiltPublisher.publish(message);
	return RETURN_SUCCESS;
}

int CPanTiltROSStub::move2MotorSync( int motorID1, int motorSpeed1, int motorPosition1, int motorID2, int motorSpeed2, int motorPosition2 ) {
	/*
	silbot3_msgs::PantiltService service;
	service.request.command = COMMAND_PANTILT_MOVE2MOTOR_SYNC;
	service.request.motorID.push_back(motorID1);
	service.request.motorID.push_back(motorID2);
	service.request.angles.push_back(motorPosition1);
	service.request.angles.push_back(motorPosition2);
	service.request.speeds.push_back(motorSpeed1);
	service.request.speeds.push_back(motorSpeed2);
	if(_pantiltClient.call(service)) {
		//ROS_INFO("[CPanTiltROSStub] COMMAND_PANTILT_MOVE2MOTOR_SYNC success");
		return RETURN_SUCCESS;
	} else {
		//ROS_ERROR("[CPanTiltROSStub] COMMAND_PANTILT_MOVE2MOTOR_SYNC fail");
		return RETURN_FAIL;
	}
	*/
	silbot3_msgs::Device_ErobotPantilt_Msg message;
	message.command = COMMAND_PANTILT_MOVE2MOTOR_SYNC;
	message.motorID.push_back(motorID1);
	message.motorID.push_back(motorID2);
	message.angles.push_back(motorPosition1);
	message.angles.push_back(motorPosition2);
	message.speeds.push_back(motorSpeed1);
	message.speeds.push_back(motorSpeed2);
	_pantiltPublisher.publish(message);
	return RETURN_SUCCESS;
}

int CPanTiltROSStub::moveMotor( int motorID, int motorSpeed, int motorPosition ) {
	/*
	silbot3_msgs::PantiltService service;
	service.request.command = COMMAND_PANTILT_MOVE_MOTOR;
	service.request.motorID.push_back(motorID);
	service.request.angles.push_back(motorPosition);
	service.request.speeds.push_back(motorSpeed);
	if(_pantiltClient.call(service)) {
		//ROS_INFO("[CPanTiltROSStub] COMMAND_PANTILT_MOVE_MOTOR success");
		return RETURN_SUCCESS;
	} else {
		//ROS_ERROR("[CPanTiltROSStub] COMMAND_PANTILT_MOVE_MOTOR fail");
		return RETURN_FAIL;
	}
	*/
	silbot3_msgs::Device_ErobotPantilt_Msg message;
	message.command = COMMAND_PANTILT_MOVE_MOTOR;
	message.motorID.push_back(motorID);
	message.angles.push_back(motorPosition);
	message.speeds.push_back(motorSpeed);
	_pantiltPublisher.publish(message);
	return RETURN_SUCCESS;
}


void CPanTiltROSStub::getPosition(CPanTiltPosition& pantiltPosition) {
	silbot3_msgs::Device_Data_Srv service;
	service.request.command = COMMAND_PANTILT_GETPOSITION;
	if(_pantiltPositionDataClient.call(service)) {
		//ROS_INFO("[CPanTiltROSStub] COMMAND_PANTILT_GETPOSITION success");
		pantiltPosition.setDegreeLeftRight(service.response.dParams.at(0));
		pantiltPosition.setDegreeUpDown(service.response.dParams.at(1));
	} else {
		//ROS_ERROR("[CPanTiltROSStub] COMMAND_PANTILT_GETPOSITION fail");
	}
}

int CPanTiltROSStub::setCompliance(int option) {
	//ROS_ERROR("[CPanTiltROSStub] COMMAND_PANTILT_SET_COMPLIANCE fail");
	silbot3_msgs::Device_ErobotPantilt_Msg message;
	message.command = COMMAND_PANTILT_SET_COMPLIANCE;
	message.speeds.push_back(option);
	_pantiltPublisher.publish(message);
	return RETURN_SUCCESS;
}

int CPanTiltROSStub::moveToABSAll(int panAngle, int tiltAngle, int xtionAngle, int panSpeed, int tiltSpeed, int xtionSpeed) {
	/*
	silbot3_msgs::PantiltService service;
	service.request.command = COMMAND_PANTILT_MOVE_ABSOLUTE_POSITION_ALL;
	service.request.angles.push_back(panAngle);
	service.request.angles.push_back(tiltAngle);
	service.request.angles.push_back(xtionAngle);
	service.request.speeds.push_back(panSpeed);
	service.request.speeds.push_back(tiltSpeed);
	service.request.speeds.push_back(xtionSpeed);
	if(_pantiltClient.call(service)) {
		//ROS_INFO("[CPanTiltROSStub] COMMAND_PANTILT_MOVE_ABSOLUTE_POSITION_ALL success");
		return RETURN_SUCCESS;
	} else {
		//ROS_ERROR("[CPanTiltROSStub] COMMAND_PANTILT_MOVE_ABSOLUTE_POSITION_ALL fail");
		return RETURN_FAIL;
	}
	*/
	silbot3_msgs::Device_ErobotPantilt_Msg message;
	message.command = COMMAND_PANTILT_MOVE_ABSOLUTE_POSITION_ALL;
	message.angles.push_back(panAngle);
	message.angles.push_back(tiltAngle);
	message.angles.push_back(xtionAngle);
	message.speeds.push_back(panSpeed);
	message.speeds.push_back(tiltSpeed);
	message.speeds.push_back(xtionSpeed);
	_pantiltPublisher.publish(message);
	return RETURN_SUCCESS;
}

void CPanTiltROSStub::emergencyStop() {
	silbot3_msgs::Device_ErobotPantilt_Msg message;
	message.command = COMMAND_PANTILT_EMERGENCY_STOP;
	_pantiltPublisher.publish(message);
}

void CPanTiltROSStub::emergencyRelease() {
	silbot3_msgs::Device_ErobotPantilt_Msg message;
	message.command = COMMAND_PANTILT_EMERGENCY_RELEASE;
	_pantiltPublisher.publish(message);
}

int CPanTiltROSStub::moveXtionTilt(int xtionAngle, int xtionSpeed) {
	silbot3_msgs::Device_ErobotPantilt_Msg message;
	message.command = COMMAND_PANTILT_MOVE_XTION_TILT;
	message.angles.push_back(xtionAngle);
	message.speeds.push_back(xtionSpeed);
	_pantiltPublisher.publish(message);
	return RETURN_SUCCESS;

}

int CPanTiltROSStub::getXtionTiltAngle() {
	return 0;
}

void CPanTiltROSStub::detectingError(short errorCode[3]) {
	silbot3_msgs::Device_Error_Srv service;
	service.request.command = COMMAND_PANTILT_DETECTING_ERROR;
	if(_pantiltErrorClient.call(service)) {
		//ROS_INFO("[CWheelROSStub] COMMAND_WHEEL_RESET success");
		for(int i=0; i<3; i++) {
			errorCode[i] = (short)service.response.iParams.at(i);
		}

	} else {
		//ROS_ERROR("[CWheelROSStub] COMMAND_WHEEL_RESET fail");
	}
}
