#include "StatusPublisher.h"

namespace expression {
namespace connector {

StatusPublisher::StatusPublisher() {
	this->publisher = nh.advertise<silbot3_msgs::ExpressionStatus>("/silbot3_expression/status", 1000);
	ros::Duration(1).sleep();
}

StatusPublisher::~StatusPublisher() {

}

StatusPublisher* StatusPublisher::getInstance() {
	static StatusPublisher instance;
	return &instance;
}

void StatusPublisher::publish() {

}

void StatusPublisher::publish(const silbot3_msgs::ExpressionStatus& msg) {
	publisher.publish(msg);
//	ros::spinOnce();
}


}
} /* namespace behavior */
