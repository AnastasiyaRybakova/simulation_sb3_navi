#ifndef __EROBOT_FUNCTION_FACTORY_H__
#define __EROBOT_FUNCTION_FACTORY_H__

#include <reactive/erobot/ERobotDefinition.h>
#include <stdio.h>
#include <iostream>

using namespace std;

namespace reactive {
	namespace erobot {
		namespace core {
			class ERobotFunctionFactory {
			
			private :
				ERobotFunctionFactory();
				virtual ~ERobotFunctionFactory();
			
			public :
				static ERobotFunctionFactory* getInstance();
				
				void intTo2Bytes(int n, unsigned char * bp);
				void intToByte(int n, unsigned char * bp);
				int byte2Int(unsigned char *bp, int len);
				void byte2Short(unsigned char *bp, int len, short *data);

				unsigned char getChecksum(unsigned char* packet, int packet_len);
				unsigned char getChecksumWithNoOffset(unsigned char* packet, int packet_len);
				unsigned char getChecksumWithStartIndex(unsigned char* packet, int packet_len, int startIndex);

				int getReadPacketSize(unsigned char nCommand);

			};
		}
	}
}

#endif /* __EROBOT_FUNCTION_FACTORY_H__ */
