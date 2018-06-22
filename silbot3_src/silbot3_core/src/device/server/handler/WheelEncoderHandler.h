#ifndef __DEVICE_SERVER_WHEELENCODER_HANDLER_H__
#define __DEVICE_SERVER_WHEELENCODER_HANDLER_H__

#include <string>
#include <ros/ros.h>

#include <device/idif/wheelencoder/WheelEncoderERobot.h>
#include <robocare/device/RosCIRDefinitions.h>

#include <silbot3_msgs/Device_WheelEncoder_Srv.h>
#include <silbot3_msgs/Device_WheelEncoder_Msg.h>

#include <geometry_msgs/Vector3.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>

using namespace std;

namespace cir {
	namespace devicenode {
		class WheelEncoderHandler {

		private :
			ros::NodeHandle			_nodeHandle;
			ros::ServiceServer		_wheelEncoderServiceServer;
			ros::Publisher			_wheelEncoderPublisher;
			ros::Publisher 			_odom_pub;
			CWheelEncoder*			_wheelEncoderServerProxy;
			bool					_WHEEL_ENCODER_THREAD_FLAG;
			boost::thread			_this_thread;

			long _PreviousLeftEncoderCounts;
			long _PreviousRightEncoderCounts;
			double DistancePerCount;

			double x;
			double y;
			double th;

			double vx;
			double vy;
			double vth;
			double deltaLeft;
			double deltaRight;

			nav_msgs::Odometry odomMsgs;
			tf::TransformBroadcaster tfBroadcaster;

		public :
			WheelEncoderHandler();
			virtual ~WheelEncoderHandler();

		private :
			void initHandler();

		public :
			bool wheelEncoderServiceCallback(silbot3_msgs::Device_WheelEncoder_Srv::Request &request, silbot3_msgs::Device_WheelEncoder_Srv::Response &response);
			void wheelEncoderPublishThread();
		};
	}
}


#endif /* __DEVICE_SERVER_WHEELENCODER_HANDLER_H__ */
