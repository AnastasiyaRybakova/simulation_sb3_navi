#ifndef __EROBOT_WHEEL_CORE_H__
#define __EROBOT_WHEEL_CORE_H__

#include <reactive/erobot/ERobotDefinition.h>
#include <reactive/erobot/session/ERobotSerialConnector.h>
#include <reactive/erobot/utility/ERobotFunctionFactory.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

using namespace std;
using namespace reactive::erobot::core;

namespace reactive {
	namespace erobot {
		namespace core {
			class EWheelCore {

			private :
				ERobotFunctionFactory* utility;
				unsigned char _current_wheel_command;
				unsigned char _current_wheel_data[EROBOT_WHEEL_DATA_PACKET_SIZE];
				int _current_wheel_param[4];

			public :
				EWheelCore();
				virtual ~EWheelCore();

				void getWheelCommandPacket(unsigned char *buffer);
				void addWheelCommandPacket(unsigned char* buffer, const int startIndex);
				void addWheelStopCommandPacket(unsigned char* buffer, const int startIndex);
				void addWheelSelfDiagnosisCommandPacket(unsigned char* buffer, const int startIndex);
				void addWheelSelfDiagnosisCommandPacket2(unsigned char* buffer, const int startIndex);

				int parseAndSetWheelDataPacket(int dataPacketSize, unsigned char* dataPacket);
				int parseAndSetWheelDataPacket(int dataPacketSize, unsigned char* dataPacket, const int startIndex);

				void setWheelValue(unsigned char command, int param1, int param2, int param3, int param4);
				bool isWheelPacket(unsigned char* buffer, const int startIndex);

				bool isWheelErrorPacket(unsigned char* buffer);
				int parseAndSetWheelErrorDataPacket(int dataPacketSize, unsigned char* dataPacket, short errorCode[9]);

				bool isWheelStop();
			};
		}
	}
}

#endif /* __EROBOT_WHEEL_CORE_H__ */

