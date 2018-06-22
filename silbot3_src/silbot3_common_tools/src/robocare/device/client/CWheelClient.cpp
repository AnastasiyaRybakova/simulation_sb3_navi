#include "CWheelClient.h"

CWheelClientProxy::CWheelClientProxy() {

}

CWheelClientProxy::~CWheelClientProxy() {

}

CWheel* CWheelClientProxy::newInstance() {
	//cout << "[CWheelClientProxy] : Create Sole Instance(wheel).." << endl;
	ROS_INFO("[CWheelClientProxy] : Create Sole Instance(CWheelROSStub)..");
	return new CWheelROSStub();
}

CWheelROSStub::CWheelROSStub() {
	this->_wheelClient = this->_nodeHandle.serviceClient<silbot3_msgs::Device_Wheel_Srv>("/DeviceNode/WheelService",true);
	this->_wheelErrorClient = this->_nodeHandle.serviceClient<silbot3_msgs::Device_Error_Srv>("/DeviceNode/WheelErrorService",true);
	this->_wheelPublisher = this->_nodeHandle.advertise<silbot3_msgs::Device_Wheel_Msg>("/DeviceNode/Wheel/commands", 1000);
	this->_wheelSubscriber = this->_nodeHandle.subscribe<silbot3_msgs::Device_Wheel_Msg>("/DeviceNode/Wheel/response", 1000, &CWheelROSStub::subscribeCallBack, this);

	this->_twistPublisher = this->_nodeHandle.advertise<geometry_msgs::Twist>("/cmd/vel", 1000);
	ROS_INFO("[CWheelROSStub] : check device node state.. ");
	while(this->_wheelPublisher.getNumSubscribers() < 1) {
		ros::Duration(0.1).sleep();
	}
	isDeltaMove = false;

	ROS_INFO("   - check finished..(OK) \n");
}

CWheelROSStub::~CWheelROSStub()
{
}

void CWheelROSStub::subscribeCallBack(const silbot3_msgs::Device_Wheel_Msg::ConstPtr& response) {


	deltaMoveResult = response->iParams.at(0);
	ROS_DEBUG(" CWheelROSStub::subscribeCallBack %d \n", deltaMoveResult);
	if(isDeltaMove) {
		if(deltaMoveResult != -99) {
			boost::mutex::scoped_lock lock(_mutex);
			ROS_DEBUG("[CWheelROSStub] CWheelROSStub::subscribeCallBack after lock");
			_condition.notify_one();
			isDeltaMove = false;
		}
	}
}

int CWheelROSStub::on() {
	ROS_DEBUG("[CWheelROSStub] on");
	/*
	silbot3_msgs::WheelService service;
	service.request.command = COMMAND_WHEEL_ON;
	if(_wheelClient.call(service)) {
		//ROS_INFO("[CWheelROSStub] COMMAND_WHEEL_ON success");
		return RETURN_SUCCESS;
	} else {
		//ROS_ERROR("[CWheelROSStub] COMMAND_WHEEL_ON fail");
		return RETURN_FAIL;
	}
	*/
	silbot3_msgs::Device_Wheel_Msg message;
	message.command = COMMAND_WHEEL_ON;
	_wheelPublisher.publish(message);
	ros::spinOnce();
	return RETURN_SUCCESS;
}


int CWheelROSStub::off() {
	ROS_DEBUG("[CWheelROSStub] off");
	/*
	silbot3_msgs::WheelService service;
	service.request.command = COMMAND_WHEEL_OFF;
	if(_wheelClient.call(service)) {
		//ROS_INFO("[CWheelROSStub] COMMAND_WHEEL_OFF success");
		return RETURN_SUCCESS;
	} else {
		//ROS_ERROR("[CWheelROSStub] COMMAND_WHEEL_OFF fail");
		return RETURN_FAIL;
	}
	*/
	silbot3_msgs::Device_Wheel_Msg message;
	message.command = COMMAND_WHEEL_OFF;
	_wheelPublisher.publish(message);
	ros::spinOnce();
	return RETURN_SUCCESS;
}

int CWheelROSStub::stop() {
	ROS_DEBUG("[CWheelROSStub] stop");
	/*
	silbot3_msgs::WheelService service;
	service.request.command = COMMAND_WHEEL_STOP;
	if(_wheelClient.call(service)) {
		//ROS_INFO("[CWheelROSStub] COMMAND_WHEEL_STOP success");
		return RETURN_SUCCESS;
	} else {
		//ROS_ERROR("[CWheelROSStub] COMMAND_WHEEL_STOP fail");
		return RETURN_FAIL;
	}
	*/
	silbot3_msgs::Device_Wheel_Msg message;
	message.command = COMMAND_WHEEL_STOP;
	_wheelPublisher.publish(message);
	ros::spinOnce();
	return RETURN_SUCCESS;
}


int CWheelROSStub::moveByTRVelocity( double translationalVelocity, double rotationalVelocity ) {
	ROS_DEBUG("[CWheelROSStub] moveByTRVelocity");
	/*
	silbot3_msgs::WheelService service;
	service.request.command = COMMAND_WHEEL_MOVE_BY_TR_VELOCITY;
	service.request.dParams.push_back(translationalVelocity);
	service.request.dParams.push_back(rotationalVelocity);
	if(_wheelClient.call(service)) {
		//ROS_INFO("[CWheelROSStub] COMMAND_WHEEL_MOVE_BY_TR_VELOCITY success");
		return RETURN_SUCCESS;
	} else {
		//ROS_ERROR("[CWheelROSStub] COMMAND_WHEEL_MOVE_BY_TR_VELOCITY fail");
		return RETURN_FAIL;
	}
	*/
	silbot3_msgs::Device_Wheel_Msg message;
	message.command = COMMAND_WHEEL_MOVE_BY_TR_VELOCITY;
	message.dParams.push_back(translationalVelocity);
	message.dParams.push_back(rotationalVelocity);
	_wheelPublisher.publish(message);
	return RETURN_SUCCESS;
}


int CWheelROSStub::moveByWheelVelocity( double leftWheelVelocity, double rightWheelVelocity ) {
	ROS_DEBUG("[CWheelROSStub] moveByWheelVelocity");
	/*
	silbot3_msgs::WheelService service;
	service.request.command = COMMAND_WHEEL_MOVE_BY_WHEEL_VELOCITY;
	service.request.dParams.push_back(leftWheelVelocity);
	service.request.dParams.push_back(rightWheelVelocity);
	if(_wheelClient.call(service)) {
		//ROS_INFO("[CWheelROSStub] COMMAND_WHEEL_MOVE_BY_WHEEL_VELOCITY success");
		return RETURN_SUCCESS;
	} else {
		//ROS_ERROR("[CWheelROSStub] COMMAND_WHEEL_MOVE_BY_WHEEL_VELOCITY fail");
		return RETURN_FAIL;
	}
	*/
	silbot3_msgs::Device_Wheel_Msg message;
	message.command = COMMAND_WHEEL_MOVE_BY_WHEEL_VELOCITY;
	message.dParams.push_back(leftWheelVelocity);
	message.dParams.push_back(rightWheelVelocity);
	_wheelPublisher.publish(message);
	return RETURN_SUCCESS;
}


int CWheelROSStub::moveToDeltaX( int deltaX, double velocity ) {
	ROS_DEBUG("[CWheelROSStub] moveToDeltaX");
	/*
	silbot3_msgs::WheelService service;
	service.request.command = COMMAND_WHEEL_MOVE_TO_DELTA_X;
	service.request.iParams.push_back(deltaX);
	service.request.dParams.push_back(velocity);
	if(_wheelClient.call(service)) {
		//ROS_INFO("[CWheelROSStub] COMMAND_WHEEL_MOVE_TO_DELTA_X success");
		return RETURN_SUCCESS;
	} else {
		//ROS_ERROR("[CWheelROSStub] COMMAND_WHEEL_MOVE_TO_DELTA_X fail");
		return RETURN_FAIL;
	}
	*/
	silbot3_msgs::Device_Wheel_Msg message;
	message.command = COMMAND_WHEEL_MOVE_TO_DELTA_X;
	message.iParams.push_back(deltaX);
	message.dParams.push_back(velocity);
	_wheelPublisher.publish(message);
	return RETURN_SUCCESS;
}


int CWheelROSStub::moveToDeltaTheta( int deltaTheta, int radius, double velocity ) {
	ROS_DEBUG("[CWheelROSStub] moveToDeltaTheta");
	/*
	silbot3_msgs::WheelService service;
	service.request.command = COMMAND_WHEEL_MOVE_TO_DELTA_THETA;
	service.request.iParams.push_back(deltaTheta);
	service.request.iParams.push_back(radius);
	service.request.dParams.push_back(velocity);
	if(_wheelClient.call(service)) {
		//ROS_INFO("[CWheelROSStub] COMMAND_WHEEL_MOVE_TO_DELTA_THETA success");
		return RETURN_SUCCESS;
	} else {
		//ROS_ERROR("[CWheelROSStub] COMMAND_WHEEL_MOVE_TO_DELTA_THETA fail");
		return RETURN_FAIL;
	}
	*/

	silbot3_msgs::Device_Wheel_Msg message;
	message.command = COMMAND_WHEEL_MOVE_TO_DELTA_THETA;
	message.iParams.push_back(deltaTheta);
	message.iParams.push_back(radius);
	message.dParams.push_back(velocity);
	_wheelPublisher.publish(message);
	return RETURN_SUCCESS;
}


int CWheelROSStub::moveToDeltaXYTheta( int deltaX, int deltaY, int deltaTheta, double velocity ) {
	ROS_DEBUG("[CWheelROSStub] moveToDeltaXYTheta");
//	silbot3_msgs::Device_Wheel_Srv service;
//	service.request.command = COMMAND_WHEEL_MOVE_TO_DELTA_XYTHETA;
//	service.request.iParams.push_back(deltaX);
//	service.request.iParams.push_back(deltaY);
//	service.request.iParams.push_back(deltaTheta);
//	service.request.dParams.push_back(velocity);
//	if(_wheelClient.call(service)) {
//		//ROS_INFO("[CWheelROSStub] COMMAND_WHEEL_MOVE_TO_DELTA_XYTHETA success");
//		return RETURN_SUCCESS;
//	} else {
//		//ROS_ERROR("[CWheelROSStub] COMMAND_WHEEL_MOVE_TO_DELTA_XYTHETA fail");
//		return RETURN_FAIL;
//	}

	silbot3_msgs::Device_Wheel_Msg message;
	message.command = COMMAND_WHEEL_MOVE_TO_DELTA_XYTHETA;
	message.iParams.push_back(deltaX);
	message.iParams.push_back(deltaY);
	message.iParams.push_back(deltaTheta);
	message.dParams.push_back(velocity);
	_wheelPublisher.publish(message);

	isDeltaMove = true;

	return RETURN_SUCCESS;

}

int CWheelROSStub::waitmoveToDeltaFinished() {
	ROS_DEBUG("[CWheelROSStub] waitmoveToDeltaFinished");

	isDeltaMove = true;
	boost::mutex::scoped_lock lock(_mutex);
	ROS_DEBUG("[CWheelROSStub] waitmoveToDeltaFinished after lock");
	_condition.wait(lock);
	ROS_DEBUG("[CWheelROSStub] DeltaFinished result %d",deltaMoveResult);
	return deltaMoveResult;
}


void CWheelROSStub::close() {
	ROS_DEBUG("[CWheelROSStub] close");
}

int CWheelROSStub::moveByVelocityXYT(double velX, double velY, double velTheta) {
	ROS_DEBUG("[CWheelROSStub] moveByVelocityXYT");
	/*
	silbot3_msgs::WheelService service;
	service.request.command = COMMAND_WHEEL_MOVE_BY_VELOCITY_XYT;
	service.request.dParams.push_back(velX);
	service.request.dParams.push_back(velY);
	service.request.dParams.push_back(velTheta);
	if(_wheelClient.call(service)) {
		//ROS_INFO("[CWheelROSStub] COMMAND_WHEEL_MOVE_BY_VELOCITY_XYT success");
		return RETURN_SUCCESS;
	} else {
		//ROS_ERROR("[CWheelROSStub] COMMAND_WHEEL_MOVE_BY_VELOCITY_XYT fail");
		return RETURN_FAIL;
	}
	*/
	silbot3_msgs::Device_Wheel_Msg message;
	message.command = COMMAND_WHEEL_MOVE_BY_VELOCITY_XYT;
	message.dParams.push_back(velX);
	message.dParams.push_back(velY);
	message.dParams.push_back(velTheta);
	_wheelPublisher.publish(message);
	return RETURN_SUCCESS;
}

int CWheelROSStub::moveByWheelVelocity( double wheel1Rpm, double wheel2Rpm, double wheel3Rpm) {
	ROS_DEBUG("[CWheelROSStub] moveByWheelVelocity");
	/*
	silbot3_msgs::WheelService service;
	service.request.command = COMMAND_WHEEL_MOVE_BY_WHEEL_VELOCITY;
	service.request.dParams.push_back(wheel1Rpm);
	service.request.dParams.push_back(wheel2Rpm);
	service.request.dParams.push_back(wheel3Rpm);
	if(_wheelClient.call(service)) {
		//ROS_INFO("[CWheelROSStub] COMMAND_WHEEL_MOVE_BY_WHEEL_VELOCITY success");
		return RETURN_SUCCESS;
	} else {
		//ROS_ERROR("[CWheelROSStub] COMMAND_WHEEL_MOVE_BY_WHEEL_VELOCITY fail");
		return RETURN_FAIL;
	}
	*/
	silbot3_msgs::Device_Wheel_Msg message;
	message.command = COMMAND_WHEEL_MOVE_BY_WHEEL_VELOCITY;
	message.dParams.push_back(wheel1Rpm);
	message.dParams.push_back(wheel2Rpm);
	message.dParams.push_back(wheel3Rpm);
	_wheelPublisher.publish(message);
	return RETURN_SUCCESS;
}
int CWheelROSStub::reset() {
	ROS_DEBUG("[CWheelROSStub] reset");
	/*
	silbot3_msgs::WheelService service;
	service.request.command = COMMAND_WHEEL_RESET;
	if(_wheelClient.call(service)) {
		//ROS_INFO("[CWheelROSStub] COMMAND_WHEEL_RESET success");
		return RETURN_SUCCESS;
	} else {
		//ROS_ERROR("[CWheelROSStub] COMMAND_WHEEL_RESET fail");
		return RETURN_FAIL;
	}
	*/
	silbot3_msgs::Device_Wheel_Msg message;
	message.command = COMMAND_WHEEL_RESET;
	_wheelPublisher.publish(message);
	ros::spinOnce();
	return RETURN_SUCCESS;
}

int CWheelROSStub::moveByTwistMsg(const geometry_msgs::Twist& twistMsg) {

	_twistPublisher.publish(twistMsg);
	return RETURN_SUCCESS;

}

int CWheelROSStub::emergencyStop() {
	ROS_DEBUG("[CWheelROSStub] emergencyStop");
	silbot3_msgs::Device_Wheel_Msg message;
	message.command = COMMAND_WHEEL_EMERGENCY_STOP;
	_wheelPublisher.publish(message);
	return RETURN_SUCCESS;
}

int CWheelROSStub::emergencyRelease() {
	ROS_DEBUG("[CWheelROSStub] emergencyRelease");
	silbot3_msgs::Device_Wheel_Msg message;
	message.command = COMMAND_WHEEL_EMERGENCY_RELEASE;
	_wheelPublisher.publish(message);
	return RETURN_SUCCESS;
}

int CWheelROSStub::emergencyMoveByVelocityXYT(double velX, double velY, double velTheta) {
	silbot3_msgs::Device_Wheel_Msg message;
	message.command = COMMAND_WHEEL_EMERGENCY_MOVE_BY_VELOCITY_XYT;
	message.dParams.push_back(velX);
	message.dParams.push_back(velY);
	message.dParams.push_back(velTheta);
	_wheelPublisher.publish(message);
	return RETURN_SUCCESS;
}

int CWheelROSStub::selfDiagnosis(short errorCode[17]) {
	silbot3_msgs::Device_Error_Srv service;
	service.request.command = COMMAND_WHEEL_SELF_DIAGNOSIS;
	if(_wheelErrorClient.call(service)) {
		//ROS_INFO("[CWheelROSStub] COMMAND_WHEEL_RESET success");
		for(int i=0; i<17; i++) {
			errorCode[i] = (short)service.response.iParams.at(i);
		}

		return RETURN_SUCCESS;
	} else {
		//ROS_ERROR("[CWheelROSStub] COMMAND_WHEEL_RESET fail");
		return RETURN_FAIL;
	}

}
