#ifndef __DEVICE_SERVER_TOUCH_HANDLER_H__
#define __DEVICE_SERVER_TOUCH_HANDLER_H__

#include <string>
#include <ros/ros.h>
#include <device/idif/touch/TouchButtonERobot.h>

#include <silbot3_msgs/Device_Error_Srv.h>
#include <silbot3_msgs/Device_Touch_Msg.h>
#include <robocare/device/RosCIRDefinitions.h>

using namespace std;

namespace cir {
	namespace devicenode {
		class TouchHandler {

		private :
			ros::NodeHandle			_nodeHandle;
			ros::Publisher			_touchPublisher;
			ros::ServiceServer		_touchErrorServiceServer;
			CTouchButton*			_touchButtonServerProxy;
			bool					_TOUCH_THREAD_FLAG;
			boost::thread			_this_thread;

		public :
			TouchHandler();
			virtual ~TouchHandler();

		private :
			void initHandler();

		public :
			void touchPublishThread();
			bool touchErrorServiceCallback(silbot3_msgs::Device_Error_Srv::Request &request, silbot3_msgs::Device_Error_Srv::Response &response);
		};
	}
}


#endif /* __DEVICE_SERVER_TOUCH_HANDLER_H__ */
