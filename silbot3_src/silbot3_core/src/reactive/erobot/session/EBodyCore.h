/*
 * EBodyCore.h
 *
 *  Created on: 2016. 1. 26.
 *      Author: zikprid
 */


#ifndef __EROBOT_BODY_CORE_H__
#define __EROBOT_BODY_CORE_H__

#include <reactive/erobot/utility/ERobotFunctionFactory.h>
#include <reactive/erobot/ERobotDefinition.h>
#include <stdio.h>
#include <iostream>

using namespace std;
using namespace reactive::erobot::core;

namespace reactive {
	namespace erobot {
		namespace core {
			class EBodyCore {

			public :

				virtual void getBodyCommandPacket(unsigned char *buffer) {}
				virtual void addBodyCommandPacket(unsigned char* buffer, const int startIndex) {}
				virtual void addGetPositionPacket(unsigned char* buffer, const int startIndex) {}

				virtual int parseAndSetBodyDataPacket(int dataPacketSize, unsigned char* dataPacket) {}
				virtual int parseAndSetBodyDataPacket(int dataPacketSize, unsigned char* dataPacket, const int startIndex) {}

				virtual void setCommandValue(short values) {}
				virtual void setLeftValue(short *values) {}
				virtual void setRightValue(short *values) {}
				virtual void setHeadValue(unsigned char *values) {}
				virtual void setHeadValueZero() {}
				virtual bool isBodyPacket(unsigned char* buffer, const int startIndex) {}

				virtual void getBodyPositons(short* positions) {}

				virtual bool isBodyErrorPacket(unsigned char* buffer) {}
				virtual int parseAndSetBodyErrorDataPacket(int dataPacketSize, unsigned char* dataPacket, short errorCode[8]) {}
				virtual bool isBodyPositionPacket(unsigned char* buffer) {}
				virtual int parseAndSetBodyPositionDataPacket(int dataPacketSize, unsigned char* dataPacket, short angle[8]) {}
			};
		}
	}
}

#endif /* __EROBOT_BODY_CORE_H__ */

