#ifndef __EROBOT_ARM_CORE_H__
#define __EROBOT_ARM_CORE_H__

#include <reactive/erobot/utility/ERobotFunctionFactory.h>
#include <reactive/erobot/ERobotDefinition.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

using namespace std;
using namespace reactive::erobot::core;

namespace reactive {
	namespace erobot {
		namespace core {
			class EArmCore {

			private :
				ERobotFunctionFactory* utility;
				unsigned char _current_arm_command;
				unsigned char _current_arm_data[EROBOT_ARM_DATA_PACKET_SIZE];

				short _current_arm_position[6];

			public :
				EArmCore();
				virtual ~EArmCore();

				void getArmCommandPacket(unsigned char *buffer);
				void addArmCommandPacket(unsigned char* buffer, const int startIndex);
				void addGetPositionPacket(unsigned char* buffer, const int startIndex);

				int parseAndSetArmDataPacket(int dataPacketSize, unsigned char* dataPacket);
				int parseAndSetArmDataPacket(int dataPacketSize, unsigned char* dataPacket, const int startIndex);

				void setArmValue(unsigned char command, short *angles, short *speeds);
				void setLeftValue(short *values);
				void setRightValue(short *values);
				bool isArmPacket(unsigned char* buffer, const int startIndex);

				void getArmPositons(short* positions);

				bool isArmErrorPacket(unsigned char* buffer);
				int parseAndSetArmErrorDataPacket(int dataPacketSize, unsigned char* dataPacket, short errorCode[6]);
				bool isArmPositionPacket(unsigned char* buffer);
				int parseAndSetArmPositionDataPacket(int dataPacketSize, unsigned char* dataPacket, short angle[6]);
			};
		}
	}
}

#endif /* __EROBOT_ARM_CORE_H__ */
