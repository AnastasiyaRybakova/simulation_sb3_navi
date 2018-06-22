#ifndef __DEVICE_SERVER_LED_HANDLER_H__
#define __DEVICE_SERVER_LED_HANDLER_H__

#include <string>
#include <ros/ros.h>
#include <device/odif/led/LEDERobot.h>

#include <silbot3_msgs/Device_LED_Msg.h>
#include <robocare/device/RosCIRDefinitions.h>

using namespace std;

namespace cir {
	namespace devicenode {
		class LEDHandler {

		private :
			ros::NodeHandle			_nodeHandle;
			ros::Subscriber			_ledSubscriber;
			CLightEmittingDiode*	_ledServerProxy;

		public :
			LEDHandler();
			virtual ~LEDHandler();

		private :
			void initHandler();

		public :
			void ledSubscribeCallback(const silbot3_msgs::Device_LED_Msg::ConstPtr& message);
		};
	}
}


#endif /* __DEVICE_SERVER_LED_HANDLER_H__ */
