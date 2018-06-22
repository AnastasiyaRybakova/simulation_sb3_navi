#ifndef __EROBOT_WHEEL_ENCODER_CORE_H__
#define __EROBOT_WHEEL_ENCODER_CORE_H__

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
			class EWheelEncoderCore {

			private :
				long	_this_wheel_encoder_encoder[EROBOT_WHEELENCODER_DATACOUNT];
				int		_this_wheel_encoder_position[EROBOT_WHEELENCODER_DATACOUNT];
				long	_init_this_wheel_encoder_encoder[EROBOT_WHEELENCODER_DATACOUNT];
				int		_init_this_wheel_encoder_position[EROBOT_WHEELENCODER_DATACOUNT];
				int		_old_wheel_encoder_position[EROBOT_WHEELENCODER_DATACOUNT];

			public :
				EWheelEncoderCore();
				virtual ~EWheelEncoderCore();

				void getWheelEncoderCommandPacket(unsigned char command, unsigned char* buffer);
				void addWheelEncoderCommandPacket(unsigned char command, unsigned char* buffer, const int startIndex);

				int parseAndSetWheelEncoderInitDataPacket(int dataPacketSize, unsigned char* dataPacket,const int startIndex);
				int parseAndSetWheelEncoderDataPacket(int dataPacketSize, unsigned char* dataPacket);
				int parseAndSetWheelEncoderDataPacket(int dataPacketSize, unsigned char* dataPacket, const int startIndex);

				void getWheelEncoderValue(long* data, int* x, int* y, int* t);
				bool isWheelEncoderPacket(unsigned char* buffer, const int startIndex);
			};
		}
	}
}

#endif /* __EROBOT_WHEEL_ENCODER_CORE_H__ */
