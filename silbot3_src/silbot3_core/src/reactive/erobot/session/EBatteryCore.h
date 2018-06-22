/*
 * EBatteryCore.h
 *
 *  Created on: 2014. 2. 14.
 *      Author: zikprid
 */

#ifndef EBATTERYCORE_H_
#define EBATTERYCORE_H_

#include <reactive/erobot/utility/ERobotFunctionFactory.h>
#include <reactive/erobot/ERobotDefinition.h>
#include <stdio.h>
#include <iostream>
#include <string.h>

using namespace std;
using namespace reactive::erobot::core;

namespace reactive {
	namespace erobot {
		namespace core {
			class EBatteryCore {

			private :
				int		_this_battery_data;

			public :
				EBatteryCore();
				virtual ~EBatteryCore();

				void getBatteryCommandPacket(unsigned char command, unsigned char* buffer);
				void addBatteryCommandPacket(unsigned char command, unsigned char* buffer, const int startIndex);

				int parseAndSetBatteryDataPacket(int dataPacketSize, unsigned char* dataPacket);
				int parseAndSetBatteryDataPacket(int dataPacketSize, unsigned char* dataPacket, const int startIndex);

				void getBatteryValue(int* voltage);
				bool isBatteryPacket(unsigned char* buffer, const int startIndex);
			};
		}
	}
}


#endif /* EBATTERYCORE_H_ */
