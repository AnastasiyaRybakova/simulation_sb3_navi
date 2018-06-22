#include "ExpressionNode.h"

using namespace expression;

ExpressionNode::ExpressionNode() {
	this->_start_expression_server	= nh.advertiseService("/silbot3_expression/start",	&ExpressionNode::startCallBack, this);
	this->_stop_expression_server			= nh.advertiseService("/silbot3_expression/stop",	&ExpressionNode::stopCallBack,this);
	this->_set_modality_subscriber = nh.subscribe<silbot3_msgs::ExpressionSetModality>("/silbot3_expression/set_modality", 1000, &ExpressionNode::modalityCallBack, this);
	boost::thread(boost::bind(&ExpressionNode::sendStatus, this));

	ROS_DEBUG("Expression node started");
}

ExpressionNode::~ExpressionNode() {

}


void ExpressionNode::sendStatus() {
	DeviceConnector::getInstance();
	ROS_DEBUG("Expression send status started");
	silbot3_msgs::ExpressionStatus msg;
	int oldStatus = -1;
	int count = 0;
	ros::Duration delay = ros::Duration(0.2);
	while(true) {
		count++;
		int status = context.getCurrentStatus();
		msg.expression_status = status;
		if(oldStatus != status) {
			oldStatus = status;
			StatusPublisher::getInstance()->publish(msg);
			count = 0;
		} else if(count >= 25) {
			StatusPublisher::getInstance()->publish(msg);
			count = 0;
		}
		delay.sleep();
	}
}

bool ExpressionNode::startCallBack(silbot3_msgs::ExpressionStart::Request& request, silbot3_msgs::ExpressionStart::Response& response) {
	ROS_DEBUG("[ExpressionNode] startCallBack called");
	ExpressionParameter parameter;
	int expression_type = request.expression_type;

	try {
		parameter.fromParameter(request.package, request.category, request.id, request.content);

		switch(expression_type) {
				case 1:
					this->context.moveToExpressionWithoutMotion(parameter, modalityUseSet);
					break;
				case 2:
					this->context.moveToExpressionWithSong(parameter, modalityUseSet);
					break;
				default:
					this->context.moveToExpressionWithMotion(parameter, modalityUseSet);
					break;
		}
	} catch(CannotChangeStatusException& e) {
		e.setMark(__FILE__, __LINE__);
		ROS_ERROR("%s\n", e.getStackTraceString().c_str());
		ROS_DEBUG("CannotChangeStatusException parameter dump \n %s", parameter.toString().c_str());
		//throw e;
		response.expression_result = 3;
		return false;
	}
	response.expression_result = 0;
	return true;

}
void ExpressionNode::modalityCallBack(const silbot3_msgs::ExpressionSetModality::ConstPtr& modality) {

		ROS_DEBUG("[ExpressionNode] modalityCallBack called");
		try {

			bool arm1	= modality->left_arm_enabled;
			bool arm2	= modality->right_arm_enabled;
			bool neck	= modality->neck_enabled;
			bool wheel	= modality->wheel_enabled;
			bool face	= modality->face_enabled;
			bool led		= modality->led_enabled;
			bool sound	= modality->sound_enabled;

			modalityUseSet.setArm1(arm1);
			modalityUseSet.setArm2(arm2);
			modalityUseSet.setNeck(neck);
			modalityUseSet.setWheel(wheel);
			modalityUseSet.setFace(face);
			modalityUseSet.setLed(led);
			modalityUseSet.setSound(sound);

			modalityUseSet.dump();

		} catch(boost::bad_lexical_cast& e) {
			ROS_ERROR("ExpressionNode::ModalityUseSet. bad lexical. %s", e.what());
		} catch(CannotChangeStatusException& e) {
			e.setMark(__FILE__, __LINE__);
			ROS_ERROR("%s\n", e.getStackTraceString().c_str());
		}
}

bool ExpressionNode::stopCallBack(std_srvs::Empty::Request& request, std_srvs::Empty::Response& response) {
	ROS_DEBUG("[ExpressionNode] stopCallBack called");
	this->context.moveToInit();
	return true;
}


#include <ros/package.h>
int main(int argc, char **argv) {

	ros::init(argc, argv, "silbot3_expression");
	ros::NodeHandle n;

	string pkgpath = ros::package::getPath("silbot3_expression");

	expression::ExpressionNode node;
	ROS_INFO("ExpressionNode started");

	ros::spin();

	return -1;
}
