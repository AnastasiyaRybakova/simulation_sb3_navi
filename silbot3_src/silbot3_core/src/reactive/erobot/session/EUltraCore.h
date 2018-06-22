#ifndef __EROBOT_ULTRA_CORE_H__
#define __EROBOT_ULTRA_CORE_H__

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <reactive/erobot/ERobotDefinition.h>
#include <reactive/erobot/utility/ERobotFunctionFactory.h>

using namespace std;

namespace reactive {
	namespace erobot {
		namespace core {
			class EUltraCore {

			private:
				unsigned short	_this_ultrasonic_data[EROBOT_ULTRASONIC_DATACOUNT+4];		

			public :
				EUltraCore();
				virtual ~EUltraCore();

				void getUltrsonicCommandPacket(unsigned char command, unsigned char* buffer);
				void addUltrsonicCommandPacket(unsigned char command, unsigned char* buffer, const int startIndex);

				int parseAndSetUltrsonicDataPacket(int dataPacketSize, unsigned char* dataPacket);
				int parseAndSetUltrsonicDataPacket(int dataPacketSize, unsigned char* dataPacket, const int startIndex);

				void getUltrasonicValue(unsigned short *data, int nDataSize);
				bool isUltrasonicPacket(unsigned char* buffer, const int startIndex);
			};
		}
	}
}

#endif /* __EROBOT_ULTRA_CORE_H__ */
