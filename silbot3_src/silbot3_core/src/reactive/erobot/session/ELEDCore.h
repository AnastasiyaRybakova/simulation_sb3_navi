#ifndef __EROBOT_LED_CORE_H__
#define __EROBOT_LED_CORE_H__

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <reactive/erobot/ERobotDefinition.h>
#include <reactive/erobot/utility/ERobotFunctionFactory.h>

using namespace std;

namespace reactive {
	namespace erobot {
		namespace core {
			class ELEDCore {

			private :
				unsigned char _current_led_command;
				unsigned char _current_led_data[EROBOT_LED_DATA_PACKET_SIZE];
				ERobotFunctionFactory* utility;

			public :
				ELEDCore();
				virtual ~ELEDCore();

				void getLEDCommandPacket( unsigned char* buffer);
				void addLEDCommandPacket( unsigned char* buffer, const int startIndex);

				int parseAndSetLEDDataPacket(int dataPacketSize, unsigned char* dataPacket);
				int parseAndSetLEDDataPacket(int dataPacketSize, unsigned char* dataPacket, const int startIndex);

				void setLEDValue(unsigned char id, unsigned char red, unsigned char green, unsigned char blue, unsigned char blink);
				bool isLEDPacket(unsigned char* buffer, const int startIndex);
			};
		}
	}
}

#endif /* __EROBOT_LED_CORE_H__ */
