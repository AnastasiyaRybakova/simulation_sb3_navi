#include "CLightEmittingDiodeClient.h"

CLightEmittingDiodeClientProxy::CLightEmittingDiodeClientProxy()
{
}


CLightEmittingDiodeClientProxy::~CLightEmittingDiodeClientProxy()
{
}

CLightEmittingDiode* CLightEmittingDiodeClientProxy::newInstance() { // throw (SocketException, ConnectException) {
	//cout << "[CLightEmittingDiodeClientProxy] : Create Sole Instance(LED).." << endl;
	return new CLightEmittingDiodeROSStub();
}

CLightEmittingDiodeROSStub::CLightEmittingDiodeROSStub() {
	this->_ledPublisher = this->_nodeHandle.advertise<silbot3_msgs::Device_LED_Msg>("/DeviceNode/LED/commands", 1000);
	ROS_INFO("[CLightEmittingDiodeROSStub] : check device node state.. ");
	while(this->_ledPublisher.getNumSubscribers() < 1) {
		ros::Duration(0.1).sleep();
	}
	ROS_INFO("   - check finished..(OK) \n");
}

CLightEmittingDiodeROSStub::~CLightEmittingDiodeROSStub()
{
}


int CLightEmittingDiodeROSStub::on( int id, int color ) {
	ROS_DEBUG("[CLightEmittingDiodeROSStub] on");
	silbot3_msgs::Device_LED_Msg message;
	message.command = COMMAND_LED_ON;
	message.id =id;
	message.bright = color;
	_ledPublisher.publish(message);

	return RETURN_SUCCESS;
}



int CLightEmittingDiodeROSStub::on(int id, unsigned char bright, unsigned char red, unsigned char green, unsigned char blue)  {
	ROS_DEBUG("[CLightEmittingDiodeROSStub] on_RGB");
	silbot3_msgs::Device_LED_Msg message;
	message.command = COMMAND_LED_ON_RGB;
	message.id =id;
	message.bright = bright;
	message.red = red;
	message.green = green;
	message.blue = blue;
	_ledPublisher.publish(message);
	return RETURN_SUCCESS;
}
