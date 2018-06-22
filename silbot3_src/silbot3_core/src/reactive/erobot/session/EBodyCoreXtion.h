/*
 * EBodyCore.h
 *
 *  Created on: 2016. 1. 26.
 *      Author: zikprid
 */


#ifndef __EROBOT_BODY_CORE_XTION_H__
#define __EROBOT_BODY_CORE_XTION_H__

#include <reactive/erobot/utility/ERobotFunctionFactory.h>
#include <reactive/erobot/ERobotDefinition.h>
#include <reactive/erobot/session/EBodyCore.h>
#include <stdio.h>
#include <iostream>

using namespace std;
using namespace reactive::erobot::core;

namespace reactive {
	namespace erobot {
		namespace core {
			class EBodyCoreXtion : public EBodyCore {

			private :
				ERobotFunctionFactory* utility;
				unsigned char _current_body_command;
				unsigned char _current_body_data[EROBOT_BODY_DATA_PACKET_SIZE2];

				short _current_body_position[8];

			public :
				EBodyCoreXtion();
				virtual ~EBodyCoreXtion();

				void getBodyCommandPacket(unsigned char *buffer);
				void addBodyCommandPacket(unsigned char* buffer, const int startIndex);
				void addGetPositionPacket(unsigned char* buffer, const int startIndex);

				int parseAndSetBodyDataPacket(int dataPacketSize, unsigned char* dataPacket);
				int parseAndSetBodyDataPacket(int dataPacketSize, unsigned char* dataPacket, const int startIndex);

				void setCommandValue(short values);
				void setLeftValue(short *values);
				void setRightValue(short *values);
				void setHeadValue(unsigned char *values);
				void setHeadValueZero();
				bool isBodyPacket(unsigned char* buffer, const int startIndex);

				void getBodyPositons(short* positions);

				bool isBodyErrorPacket(unsigned char* buffer);
				int parseAndSetBodyErrorDataPacket(int dataPacketSize, unsigned char* dataPacket, short errorCode[8]);
				bool isBodyPositionPacket(unsigned char* buffer);
				int parseAndSetBodyPositionDataPacket(int dataPacketSize, unsigned char* dataPacket, short angle[8]);
			};
		}
	}
}

#endif /* __EROBOT_BODY_CORE_XTION_H__ */

