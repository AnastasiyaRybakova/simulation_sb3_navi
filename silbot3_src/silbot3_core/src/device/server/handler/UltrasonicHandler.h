#ifndef __DEVICE_SERVER_ULTRASONIC_HANDLER_H__
#define __DEVICE_SERVER_ULTRASONIC_HANDLER_H__

#include <string>
#include <ros/ros.h>

#include <device/idif/ultrasonic/UltrasonicERobot.h>
#include <robocare/device/RosCIRDefinitions.h>

#include <silbot3_msgs/Device_Ultra_Srv.h>
#include <silbot3_msgs/Device_Ultra_Msg.h>

using namespace std;

namespace cir {
	namespace devicenode {
		class UltrasonicHandler {

		private :
			ros::NodeHandle			_nodeHandle;
			ros::ServiceServer		_ultrasonicServiceServer;
			ros::Publisher			_ultraPublisher;
			CUltrasonic*			_ultrasonicServerProxy;
			bool					_ULTRA_THREAD_FLAG;
			boost::thread			_this_thread;

		public :
			UltrasonicHandler();
			virtual ~UltrasonicHandler();

		private :
			void initHandler();

		public :
			bool ultrasonicServiceCallback(silbot3_msgs::Device_Ultra_Srv::Request &request, silbot3_msgs::Device_Ultra_Srv::Response &response);
			void ultraPublishThread();
		};
	}
}


#endif /* __DEVICE_SERVER_ULTRASONIC_HANDLER_H__ */
