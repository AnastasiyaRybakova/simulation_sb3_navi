#include "CUltraClient.h"

CUltrasonicClientProxy::CUltrasonicClientProxy(){
}

CUltrasonicClientProxy::~CUltrasonicClientProxy() {
}


CUltrasonic* CUltrasonicClientProxy::newInstance() {
	//cout << "[CUltrasonicClientProxy] : Create Sole Instance(Ultrasonic).." << endl;
	ROS_INFO("[CUltrasonicClientProxy] : Create Sole Instance(CUltraROSStub)..");
	return new CUltraROSStub();
}

CUltraROSStub::CUltraROSStub() {
	this->_ultraSubscriber = this->_nodeHandle.subscribe<silbot3_msgs::Device_Ultra_Msg>("/DeviceNode/UltraSonic/data", 1000, &CUltraROSStub::subscribeCallBack, this);
	ROS_INFO("[CUltraROSStub] : check device node state.. ");
	while(this->_ultraSubscriber.getNumPublishers() < 1) {
		ros::Duration(0.1).sleep();
	}
	ROS_INFO("   - check finished..(OK) \n");
}


CUltraROSStub::~CUltraROSStub() {

}

void CUltraROSStub::close() {

}

CUltrasonicData CUltraROSStub::readData(int index) {
	/*
	silbot3_msgs::UltraService service;
	service.request.command = COMMAND_ULTRA_READ_DATA;
	if(_ultraClient.call(service)) {
		//ROS_INFO("[CUltraROSStub] COMMAND_ULTRA_READ_DATA success");
		unsigned short tempData[12] = {
			service.response.msg.param.at(0),
			service.response.msg.param.at(1),
			service.response.msg.param.at(2),
			service.response.msg.param.at(3),
			service.response.msg.param.at(4),
			service.response.msg.param.at(5),
			service.response.msg.param.at(6),
			service.response.msg.param.at(7),
			service.response.msg.param.at(8),
			service.response.msg.param.at(9),
			service.response.msg.param.at(10),
			service.response.msg.param.at(11),
		};
		//return RETURN_SUCCESS;
		CUltrasonicData returnData(0, 0, tempData);
		return returnData;
	} else {
		//ROS_ERROR("[CUltraROSStub] COMMAND_ULTRA_READ_DATA fail");
		//return RETURN_FAIL;
		CUltrasonicData garbage;
		return garbage;
	}
	*/
	CUltrasonicData returnData(0, 0, tempData);
	return returnData;
}

void CUltraROSStub::subscribeCallBack(const silbot3_msgs::Device_Ultra_Msg::ConstPtr& message) {
	this->tempData[0] = message->param.at(0);
	this->tempData[1] = message->param.at(1);
	this->tempData[2] = message->param.at(2);
	this->tempData[3] = message->param.at(3);
	this->tempData[4] = message->param.at(4);
	this->tempData[5] = message->param.at(5);
	this->tempData[6] = message->param.at(6);
	this->tempData[7] = message->param.at(7);
	this->tempData[8] = message->param.at(8);
	this->tempData[9] = message->param.at(9);
	this->tempData[10] = message->param.at(10);
	this->tempData[11] = message->param.at(11);
}
