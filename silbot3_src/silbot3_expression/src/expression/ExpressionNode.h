#ifndef __EXPRESSION_NODE_H__
#define __EXPRESSION_NODE_H__

#include <ros/ros.h>

#include <std_msgs/Float32.h>
#include <silbot3_msgs/ExpressionStart.h>
#include <silbot3_msgs/ExpressionSetModality.h>
#include <silbot3_msgs/ExpressionStatus.h>
#include <std_srvs/Empty.h>

#include <expression/status/Context.h>
#include <expression/modality/Modality.h>
#include <expression/modality/ModalityUseSet.h>
#include <expression/exception/InvalidProtocolException.h>
#include <expression/protocol/ExpressionParameter.h>

#include <expression/exception/ParseException.h>
#include <expression/exception/NoSuchElementException.h>
#include <expression/connector/DeviceConnector.h>
#include <expression/connector/StatusPublisher.h>

using namespace expression;
using namespace expression::status;
using namespace expression::modality;

namespace expression {

	class ExpressionNode {

	private:
		Context context;
		ModalityUseSet modalityUseSet;
		ros::NodeHandle nh;
		ros::ServiceServer _start_expression_server;
		ros::ServiceServer _stop_expression_server;
		ros::Subscriber _set_modality_subscriber;

		boost::shared_mutex _access;

	public:
		ExpressionNode();
		virtual ~ExpressionNode();

		bool startCallBack(silbot3_msgs::ExpressionStart::Request& request, silbot3_msgs::ExpressionStart::Response& response);
		void modalityCallBack(const silbot3_msgs::ExpressionSetModality::ConstPtr& modality);
		bool stopCallBack(std_srvs::Empty::Request& request, std_srvs::Empty::Response& response);
		void sendStatus();

	};

} /* namespace expression */

#endif /* __EXPRESSION_NODE_H__ */
