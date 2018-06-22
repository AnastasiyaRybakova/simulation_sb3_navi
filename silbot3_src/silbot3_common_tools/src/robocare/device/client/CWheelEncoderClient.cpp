#include "CWheelEncoderClient.h"

CWheelEncoderClientProxy::CWheelEncoderClientProxy()
{
}


CWheelEncoderClientProxy::~CWheelEncoderClientProxy()
{
}

CWheelEncoder* CWheelEncoderClientProxy::newInstance() {
	//cout << "[CWheelEncoderClientProxy] : Create Sole Instance(wheel encoder).." << endl;
	ROS_INFO("[CWheelEncoderClientProxy] : Create Sole Instance(CWheelEncoderROSStub)..");
	return new CWheelEncoderROSStub();
}

CWheelEncoderROSStub::CWheelEncoderROSStub() {
	this->_wheelEncoderSubscriber = this->_nodeHandle.subscribe<silbot3_msgs::Device_WheelEncoder_Msg>("/DeviceNode/WheelEncoder/data", 1000, &CWheelEncoderROSStub::subscribeCallBack, this);

	ROS_INFO("[CWheelEncoderROSStub] : check device node state : publisher number : %d ", _wheelEncoderSubscriber.getNumPublishers());
	while(_wheelEncoderSubscriber.getNumPublishers() < 1) {
		ROS_DEBUG("checking encoder publisher numbers1");
		ros::Duration(0.1).sleep();
		ROS_DEBUG("checking encoder publisher numbers2");
	}
	this->_this_x = 0.0;
	this->_this_y = 0.0;
	this->_this_t = 0.0;
	ROS_INFO("   - check finished..(OK) \n");
}

CWheelEncoderROSStub::~CWheelEncoderROSStub()
{
}

CWheelEncoderData CWheelEncoderROSStub::readData( int index ) {
	/*
	silbot3_msgs::WheelEncoderService service;
	service.request.command = COMMAND_WHEEL_ENCODER_READ_DATA;
	if(_wheelEncoderClient.call(service)) {
		//ROS_INFO("[CWheelEncoderROSStub] COMMAND_WHEEL_ENCODER_READ_DATA success");
		long tempData[3] = {0,0,0};
		CWheelEncoderData returnData(
			tempData,
			service.response.msg.x,
			service.response.msg.y,
			service.response.msg.t,
			CWheelEncoderData::NUMBER_OF_DATA
		);
		return returnData;
	} else {
		//ROS_ERROR("[CWheelEncoderROSStub] COMMAND_WHEEL_ENCODER_READ_DATA fail");
		//return RETURN_FAIL;
		CWheelEncoderData garbage;
		return garbage;
	}
	*/
	long tempData[3] = {0,0,0};
	CWheelEncoderData returnData(
		tempData,
		this->_this_x,
		this->_this_y,
		this->_this_t,
		CWheelEncoderData::NUMBER_OF_DATA
		);
	return returnData;
}

void CWheelEncoderROSStub::subscribeCallBack(const silbot3_msgs::Device_WheelEncoder_Msg::ConstPtr& message) {
	this->_this_x = message->x;
	this->_this_y = message->y;
	this->_this_t = message->t;
}
