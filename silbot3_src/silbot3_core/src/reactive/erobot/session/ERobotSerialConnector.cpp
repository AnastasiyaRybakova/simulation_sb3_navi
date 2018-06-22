#include "ERobotSerialConnector.h"

using namespace reactive::erobot::core;

ERobotSerialConnector::ERobotSerialConnector() {
	_bInited = false;
	_fd = -1;
}

ERobotSerialConnector::~ERobotSerialConnector() {
	if (_bInited){
			tcsetattr(_fd,TCSANOW,&oldoptions);
			::close(_fd);
		_bInited = false;
	}
}

void ERobotSerialConnector::setup(string portName, int baudrate) {

	_bInited = false;

	//lets account for the name being passed in instead of the device path
	if( portName.size() > 5 && portName.substr(0, 5) != "/dev/" ){
		portName = "/dev/" + portName;
	}

	//ofLog(OF_LOG_NOTICE,"ofSerialInit: opening port %s @ %d bps", portName.c_str(), baud);
	printf("ofSerialInit: opening port %s @ %d bps\n", portName.c_str(), baudrate);
	//fd = open(portName.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK );
	//fd = open(portName.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
	_fd = open(portName.c_str(), O_RDWR | O_NOCTTY |O_NDELAY);
	if(_fd == -1){
		//ofLog(OF_LOG_ERROR,"ofSerial: unable to open port %s", portName.c_str());
		printf("ofSerial: unable to open port %s\n", portName.c_str());
		return;
	}

	struct termios options;
	tcgetattr(_fd,&oldoptions);
	options = oldoptions;
	switch(baudrate){
	   case 300:
		   cfsetispeed(&options,B300);
		   cfsetospeed(&options,B300);
		   break;
	   case 1200:
		   cfsetispeed(&options,B1200);
		   cfsetospeed(&options,B1200);
		   break;
	   case 2400:
		   cfsetispeed(&options,B2400);
		   cfsetospeed(&options,B2400);
		   break;
	   case 4800:
		   cfsetispeed(&options,B4800);
		   cfsetospeed(&options,B4800);
		   break;
	   case 9600:
		   cfsetispeed(&options,B9600);
		   cfsetospeed(&options,B9600);
		   break;
	   case 19200:
		   cfsetispeed(&options,B19200);
		   cfsetospeed(&options,B19200);
		   break;
	   case 38400:
		   cfsetispeed(&options,B38400);
		   cfsetospeed(&options,B38400);
		   break;
	   case 57600:
		   cfsetispeed(&options,B57600);
		   cfsetospeed(&options,B57600);
		   break;
	   case 115200:
		   cfsetispeed(&options,B115200);
		   cfsetospeed(&options,B115200);
		   break;
	   case 230400:
		   cfsetispeed(&options,B230400);
		   cfsetospeed(&options,B230400);
		   break;

	   default:
		   cfsetispeed(&options,B9600);
		   cfsetospeed(&options,B9600);
		   //ofLog(OF_LOG_ERROR,"ofSerialInit: cannot set %i baud setting baud to 9600", baud);
		   break;
	}

	options.c_cflag |= (CLOCAL | CREAD);
	options.c_cflag &= ~(PARENB|PARODD);
	options.c_cflag &= ~CSTOPB;
	options.c_cflag &= ~CSIZE;
	options.c_cflag |= CS8;
	options.c_iflag = IGNBRK;
	options.c_iflag &= ~(IXON | IXOFF | IXANY);
	//ijpark
	options.c_oflag=0;
	options.c_lflag=0; //IGNPAR;
	options.c_cc[VTIME]     = 0;
	options.c_cc[VMIN]     =  0;
//		tcflush(fd,TCIFLUSH);
	tcsetattr(_fd,TCSANOW,&options);

	_bInited = true;
	//ofLog(OF_LOG_NOTICE,"success in opening serial connection");
	printf("success in opening serial connection\n");
}

int ERobotSerialConnector::readBytes( unsigned char* buffer, int length ) {
	if (!_bInited){
		 printf("ofSerial: serial not inited\n");
		return -1;
	}
	
	int nRead = read(_fd, buffer, length);
	if(nRead < 0){
		if ( errno == EAGAIN )
			return -1;
		printf("ofSerial: trouble reading from port, errno %i (%s)\n", errno, strerror(errno));
		return -1;
	}
	return nRead;
}

bool ERobotSerialConnector::writeBytes( unsigned char* buffer, int length ) {
	if (!_bInited){
		 printf("ofSerial: serial not inited\n");
		return -1;
		//return OF_SERIAL_ERROR;
	}

	int numWritten = write(_fd, buffer, length);
	if(numWritten <= 0){
		if ( errno == EAGAIN )
			return 0;
		printf("ofSerial: Can't write to com port, errno %i (%s)", errno, strerror(errno));
		return -1;
	}
	//printf("ofSerial: numWritten %i\n", numWritten);
	return numWritten;
}

void ERobotSerialConnector::resetConnection() {
	if (!_bInited){
		 printf("ofSerial: serial not inited\n");
		return;
	}

	int flushType = 0;
//	if( flushIn && flushOut) flushType = TCIOFLUSH;
//	else if(flushIn) flushType = TCIFLUSH;
//	else if(flushOut) flushType = TCOFLUSH;
//	else return;

	tcflush(_fd, TCIOFLUSH);
}
