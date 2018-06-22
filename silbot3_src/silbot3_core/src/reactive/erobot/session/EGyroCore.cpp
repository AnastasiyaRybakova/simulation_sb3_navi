// Microinfinity Cia. Ltd.
// This program demonstrates how to communicate with the CruizCore R1050 gyroscope
// This program assumes that the port 'COMM_PORT' has been properly initialized.
// In linux this can be done is Linux using the following command line:
// 			stty -F /dev/ttyUSB0 115200 raw
// This program can be compiled using:
// 			g++ -o ccr1050 ccr1050.cpp

#include <iostream>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <linux/usbdevice_fs.h>

#include <ros/ros.h>
#include "EGyroCore.h"

using namespace std;
using namespace reactive::erobot::core;


//Define constants
string comPort = "/dev/ttyUSB0";
const int PACKET_SIZE = 8;
const int SAMPLES = 1000;

//Define global variables
int file_descriptor;


EGyroCore::EGyroCore() {
	init();

	_angleOffset = 0.0;
	fistFlag = true;


	ros::NodeHandle nh("~");

	nh.param<std::string>("port/gyro", comPort, "ttyUSB0");
	comPort = "/dev/"+comPort;
}




void EGyroCore::init() {
	system("stty -F /dev/ttyUSB0 115200 raw");
	if(-1 == (file_descriptor = open(comPort.c_str(),O_RDWR) ))
	//if(-1 == (file_descriptor = open(COMM_PORT,O_WRONLY)))
	{
		cout << "Error opening port \n";
		cout << "Set port parameters using the following Linux command:\n stty -F /dev/ttyUSB0 115200 raw\n";
		cout << "You may need to have ROOT access";

		return;
	}

	struct termios oldtio,options;
	tcgetattr(file_descriptor,&oldtio);
//	$MIB,RESET*87
	unsigned char cmd[] = {'$','M','I','B',',','R', 'E','S','E','T','*','8','7','\r','\0'};
	//unsigned char cmd[] = {0x24,0x4D,0x49,0x42,0x2C,0x52,0x45,0x53,0x45,0x54,0x2A,0x38,0x37};
	int n_written = write( file_descriptor, cmd, sizeof(cmd) -1 );

	options = oldtio;

   cfsetispeed(&options,B115200);
   cfsetospeed(&options,B115200);

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
	options.c_cc[VMIN]     =  1;
//		tcflush(fd,TCIFLUSH);
	tcsetattr(file_descriptor,TCSANOW,&options);

	int rc = ioctl(file_descriptor, USBDEVFS_RESET, 0);
	cout << "CruizCoreR1050 communication port is ready\n";

	this->_this_thread = boost::thread(boost::bind(&EGyroCore::run,this));

}

void EGyroCore::ccr1050_getvalue() {
	short header;
	short rate_int;
	short angle_int;
	float rate_float;
	float angle_float;
	short check_sum;
	unsigned char data_packet[PACKET_SIZE];


	findHeader(0);
	readValue(0);

}


void EGyroCore::run() {
	while(ros::ok()) {
		ccr1050_getvalue();
	}
}


bool EGyroCore::findHeader(const int& fd) {

	unsigned char buffer;
	int readsize = 0;
	bool found = false;
	while(!found) {
		read(file_descriptor, &buffer, 1);
		if(buffer == 0xFF) {
			read(file_descriptor, &buffer, 1);
			if(buffer == 0xFF) {
				found = true;
			}
		}
	}
}

bool EGyroCore::readValue(const int& fd) {
	unsigned char buffer;
	unsigned char data_packet[6];
	int readsize = 0;
	int trace = 0;

	while(trace < 6) {
		readsize = read(file_descriptor, &buffer, 1);
		data_packet[trace++] = buffer;
	}

	short rate_int = 0 ;
	short angle_int = 0;
	short check_sum = 0;

	memcpy(&rate_int,data_packet, sizeof(short));
	memcpy(&angle_int,data_packet+2, sizeof(short));
	memcpy(&check_sum,data_packet+4, sizeof(short));


	short expectedChecksum = (short)(0xFFFF + rate_int + angle_int);
	// Verify checksum
	if(check_sum != expectedChecksum)
	{
		//cout<< "Checksum error!!\n";
		ROS_ERROR("Checksum error : expected(%d), observed(%d)", expectedChecksum, check_sum);
		return false;
	}

	float angle_float = -angle_int / 100.;
	float rate_float = rate_int / 100.;

	if(fistFlag) {
		_angleOffset = angle_float;
		fistFlag = false;
	}

	//this->_angle = angle_float - _angleOffset;
	this->_angle = angle_float;
	this->_rate = rate_float;

	ROS_DEBUG("+OK : Gyro : angle(%f), rate(%f), checksum(%d)", angle_float, rate_float, check_sum);


	return true;

}


EGyroCore::~EGyroCore() {
	close(file_descriptor);
	ROS_INFO("Closing communication port");
}

CGyroData EGyroCore::getData() {
	CGyroData d(_rate, (double)_angle);
	return d;
}

//int main(int argc, char **argv) {
//	ros::init(argc, argv, "gyro_class_test");
//	ros::NodeHandle nh;
//
//	EGyroCore core;
//	ros::spin();
//}
