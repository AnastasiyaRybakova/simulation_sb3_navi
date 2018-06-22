/*
 * BatteryHandler.h
 *
 *  Created on: 2014. 2. 14.
 *      Author: zikprid
 */

#ifndef __DEVICE_SERVER_BATTERY_HANDLER_H_
#define __DEVICE_SERVER_BATTERY_HANDLER_H_

#include <string>
#include <ros/ros.h>

#include <device/idif/battery/BatteryERobot.h>
#include <robocare/device/RosCIRDefinitions.h>

#include <silbot3_msgs/Device_Battery_Msg.h>

using namespace std;

namespace cir {
	namespace devicenode {
		class BatteryHandler {

		private :
			ros::NodeHandle			_nodeHandle;
			ros::Publisher			_batteryPublisher;
			CBattery*				_batteryServerProxy;
			bool					_BATTERY_THREAD_FLAG;
			boost::thread			_this_thread;

		public :
			BatteryHandler();
			virtual ~BatteryHandler();

		private :
			void initHandler();

		public :
			void batteryPublishThread();
		};
	}
}


#endif /* __DEVICE_SERVER_BATTERY_HANDLER_H_ */
