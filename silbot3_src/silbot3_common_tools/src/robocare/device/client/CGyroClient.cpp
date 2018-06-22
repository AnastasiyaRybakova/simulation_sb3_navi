#include "CGyroClient.h"

CGyroClientProxy::CGyroClientProxy(){
}

CGyroClientProxy::~CGyroClientProxy() {
}

CGyro* CGyroClientProxy::newInstance() {
	//cout << "[CGyroClientProxy] : Create Sole Instance(Gyro).." << endl;
	ROS_INFO("[CGyroClientProxy] : Create Sole Instance(CGyroROSStub)..");
	return new CGyroROSStub();
}

CGyroROSStub::CGyroROSStub() {
	this->_gyroSubscriber = this->_nodeHandle.subscribe<silbot3_msgs::Device_Gyro_Msg>("/DeviceNode/Gyro/data", 1000, &CGyroROSStub::subscribeCallBack, this);

	ROS_INFO("[CGyroROSStub] : check device node state.. ");
	while(this->_gyroSubscriber.getNumPublishers() < 1) {
		ros::Duration(0.1).sleep();
	}
	this->_this_angle = 0.0;
	this->_this_rate  = 0.0;
	ROS_INFO("   - check finished..(OK) \n");

}

CGyroROSStub::~CGyroROSStub() {
}

CGyroData CGyroROSStub::readData() {
	/*
	silbot3_msgs::GyroService service;
	service.request.command = COMMAND_GYRO_READ_DATA;

	if(_gyroClient.call(service)) {
		//ROS_INFO("[CGyroROSStub] COMMAND_GYRO_READ_DATA success");
		CGyroData garbage(service.response.msg.rate, service.response.msg.angle);
		return garbage;
	} else {
		//ROS_ERROR("[CGyroROSStub] COMMAND_GYRO_READ_DATA fail");
		CGyroData garbage(-1, -1);
		return garbage;
	}
	*/
	CGyroData returnData(this->_this_rate, this->_this_angle);
	return returnData;
}

void CGyroROSStub::subscribeCallBack(const silbot3_msgs::Device_Gyro_Msg::ConstPtr& message) {
	this->_this_angle = message->angle;
	this->_this_rate  = message->rate;
}
