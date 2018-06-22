#ifndef __EXPRESSION_CONNECTOR_STATUS_PUBLISHER_H__
#define __EXPRESSION_CONNECTOR_STATUS_PUBLISHER_H__

#include <string>

#include <ros/ros.h>
#include <boost/thread.hpp>

#include <expression/exception/IOException.h>

#include <silbot3_msgs/ExpressionStatus.h>

using namespace std;
using namespace expression::exception;

namespace expression {
	namespace connector {
		class StatusPublisher {
		private:
			ros::NodeHandle nh;
			ros::Publisher publisher;

		public:
			StatusPublisher();
			virtual ~StatusPublisher();

			static StatusPublisher* getInstance();
			void publish();
			void publish(const silbot3_msgs::ExpressionStatus& msg);
		};
	}
} /* namespace expression */

#endif /* __EXPRESSION_CONNECTOR_STATUS_PUBLISHER_H__ */
