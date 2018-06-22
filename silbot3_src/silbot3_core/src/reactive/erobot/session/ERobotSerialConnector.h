#ifndef __EROBOT_SERIAL_CONNECTOR_H__
#define __EROBOT_SERIAL_CONNECTOR_H__

#include <sys/ioctl.h>
#include <getopt.h>
#include <dirent.h>

#include <termios.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>
#include <algorithm>
#include <stdio.h>
#include <unistd.h>
#include <cstring>
#include <stdio.h>
#include <iostream>
#include <pthread.h>


#define READ_TIMEOUT	500

using namespace std;

namespace reactive {
	namespace erobot {
		namespace core {

			class ERobotSerialConnector {

			private :
				bool	_bInited;
				int		_fd;
				string	_baseComPort;
				string  _bodyComPort;
				struct 			termios oldoptions;

			public :			
				ERobotSerialConnector();
				virtual ~ERobotSerialConnector();
				void setup(string portName, int baudrate);

				bool writeBytes(unsigned char* buffer, int length);
				int readBytes(unsigned char* buffer, int length);

				void resetConnection();
			};
		}
	}
}

#endif /* __EROBOT_SERIAL_CONNECTOR_H__ */
