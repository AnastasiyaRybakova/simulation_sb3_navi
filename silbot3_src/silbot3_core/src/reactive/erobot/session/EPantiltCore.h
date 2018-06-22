#ifndef __EROBOT_PANTILT_CORE_H__
#define __EROBOT_PANTILT_CORE_H__


#include <reactive/erobot/utility/ERobotFunctionFactory.h>
#include <reactive/erobot/ERobotDefinition.h>
#include <stdio.h>
#include <iostream>

using namespace std;
using namespace reactive::erobot::core;

namespace reactive {
	namespace erobot {
		namespace core {
			class EPantiltCore {

			private :

				ERobotFunctionFactory* utility;
				unsigned char _current_pantilt_command;
				unsigned char _current_pantilt_data[EROBOT_PANTILT_DATA_PACKET_SIZE];

				int panAngle;
				int tiltAngle;
				int xtionAngle;

			public :
				EPantiltCore();
				virtual ~EPantiltCore();

				void getPantiltCommandPacket(unsigned char *buffer);
				void addPantiltCommandPacket(unsigned char* buffer, const int startIndex);
				void addGetPositionPacket(unsigned char* buffer, const int startIndex);

				int parseAndSetPantiltDataPacket(int dataPacketSize, unsigned char* dataPacket);
				int parseAndSetPantiltDataPacket(int dataPacketSize, unsigned char* dataPacket, const int startIndex);

				void setPantiltValue(unsigned char id, short anglePan, short angleTilt,	short angleXtionTilt, short speedPan, short speedTilt, short speenXtionTilt);
				void setHeadValue(unsigned char *values);
				bool isPantiltPacket(unsigned char* buffer, const int startIndex);

				void getPantiltPositons(double* positions);

				bool isPantiltErrorPacket(unsigned char* buffer);
				int parseAndSetPantiltErrorDataPacket(int dataPacketSize, unsigned char* dataPacket, short errorCode[3]);

				bool isPantiltPositionPacket(unsigned char* buffer);
				int parseAndSetPantiltPositionDataPacket(int dataPacketSize, unsigned char* dataPacket, double angle[3]);
			};
		}
	}
}

#endif /* __EROBOT_PANTILT_CORE_H__ */
