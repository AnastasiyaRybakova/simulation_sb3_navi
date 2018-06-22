#ifndef __EROBOT_TOUCH_CORE_H__
#define __EROBOT_TOUCH_CORE_H__

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <reactive/erobot/ERobotDefinition.h>
#include <reactive/erobot/utility/ERobotFunctionFactory.h>

using namespace std;

namespace reactive {
	namespace erobot {
		namespace core {
			class ETouchCore {
			
			private :
				unsigned short	_this_touch_data[EROBOT_TOUCH_DATACOUNT];
			
			public :
				ETouchCore();
				virtual ~ETouchCore();

				void getTouchButtonCommandPacket(unsigned char command, unsigned char* buffer);
				void addTouchButtonCommandPacket(unsigned char command, unsigned char* buffer, const int startIndex);
				void addTouchButtonErrorPacket(unsigned char command, unsigned char* buffer, const int touchId, const int startIndex);
				int parseAndSetTouchButtonDataPacket(int dataPacketSize, unsigned char* dataPacket);
				int parseAndSetTouchButtonDataPacket(int dataPacketSize, unsigned char* dataPacket, const int startIndex);

				void getTouchButtonValue(unsigned short *data, int nDataSize);
				bool isTouchButtonPacket(unsigned char* buffer, const int startIndex);

				bool isTouchErrorPacket(unsigned char* buffer);
				int parseAndSetTouchErrorDataPacket(int dataPacketSize, unsigned char* dataPacket);
			};
		}
	}
}

#endif /* __EROBOT_TOUCH_CORE_H__ */
