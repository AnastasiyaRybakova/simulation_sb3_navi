#include "CTouchButtonClient.h"

CTouchButtonClientProxy::CTouchButtonClientProxy()
{
}


CTouchButtonClientProxy::~CTouchButtonClientProxy()
{
}

CTouchButton* CTouchButtonClientProxy::newInstance() {
	return new CTouchButtonROSStub();
}

CTouchButtonROSStub::CTouchButtonROSStub() {

	//Init data
	for(int i =0; i< NUMBER_OF_DATA; i++){
		data[i] = 0;
	}

	this->_toubhButtonSubscriber = this->_nodeHandle.subscribe<silbot3_msgs::Device_Touch_Msg>("/DeviceNode/TouchButton/data", 1000, &CTouchButtonROSStub::subscribeCallBack, this);
	this->_toubhButtonErrorClient = this->_nodeHandle.serviceClient<silbot3_msgs::Device_Error_Srv>("/DeviceNode/TouchErrorService",true);
	ROS_INFO("[CTouchROSStub] : check device node state.. ");

	while(this->_toubhButtonSubscriber.getNumPublishers() < 1) {
		ros::Duration(0.1).sleep();
	}
	ROS_INFO("   - check finished..(OK) \n");

}

CTouchButtonROSStub::~CTouchButtonROSStub()
{
}


CTouchButtonData CTouchButtonROSStub::readData( int index ) {

	CTouchButtonData returnData(0, 0, data);
	return returnData;
}

CHeadTouchButtonData CTouchButtonROSStub::readHeadData( int index ) {
	CHeadTouchButtonData garbage;
	return garbage;
}


void CTouchButtonROSStub::subscribeCallBack(const silbot3_msgs::Device_Touch_Msg::ConstPtr& message)
{
	this->data[0] = message->param.at(0);
	this->data[1] = message->param.at(1);
	this->data[2] = message->param.at(2);
	this->data[3] = message->param.at(3);
	this->data[4] = message->param.at(4);
}

int CTouchButtonROSStub::detectingError(int touchID) {
	int result = -1;
	silbot3_msgs::Device_Error_Srv service;
	stringstream ss;
	ss << touchID;
	string strID = ss.str();
	service.request.command = strID;
	if(_toubhButtonErrorClient.call(service)) {
		//ROS_INFO("[CTouchButtonROSStub] COMMAND_TOUCHBUTTON_DETECTING_ERROR success");
		result = (short)service.response.iParams.at(0);
	} else {
		//ROS_ERROR("[CTouchButtonROSStub] COMMAND_TOUCHBUTTON_DETECTING_ERROR fail");
	}
	return result;
}
