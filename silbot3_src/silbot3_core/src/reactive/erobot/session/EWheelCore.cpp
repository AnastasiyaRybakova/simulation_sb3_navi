#include "EWheelCore.h"

using namespace std;
using namespace reactive::erobot::core;

EWheelCore::EWheelCore() {
	_current_wheel_command = 0x00;
	memset(_current_wheel_data, 0x00, sizeof(unsigned char) * EROBOT_WHEEL_DATA_PACKET_SIZE);
	utility = ERobotFunctionFactory::getInstance();
	_current_wheel_param[0] = 0;
	_current_wheel_param[1] = 0;
	_current_wheel_param[2] = 0;
	_current_wheel_param[3] = 0;

}

EWheelCore::~EWheelCore() {

}

void EWheelCore::getWheelCommandPacket(unsigned char *buffer) {
		
	int nCheckSumSize	= EROBOT_CHECKSUM_PACKET_SIZE; 
	int nBodySize		= EROBOT_COMMON_HEADER_PACKET_SIZE + EROBOT_WHEEL_DATA_PACKET_SIZE + nCheckSumSize;

	memset(buffer, 0x00, nBodySize);

	unsigned char checksum = utility->getChecksumWithNoOffset(_current_wheel_data, EROBOT_WHEEL_DATA_PACKET_SIZE);

	buffer[0]	= EROBOT_DEVICE_WHEEL_ID;
	buffer[1]	= EROBOT_DEVICE_WHEEL_SUB_ID_1;
	buffer[2]	= EROBOT_DEVICE_WHEEL_SUB_ID_2;
	buffer[3]	= _current_wheel_command;
	buffer[4]	= _current_wheel_data[0];
	buffer[5]	= _current_wheel_data[1];
	buffer[6]	= _current_wheel_data[2];
	buffer[7]	= _current_wheel_data[3];
	buffer[8]	= _current_wheel_data[4];
	buffer[9]	= _current_wheel_data[5];
	buffer[10]	= _current_wheel_data[6];
	buffer[11]	= checksum;
}

void EWheelCore::addWheelCommandPacket(unsigned char* buffer, const int startIndex) {

	unsigned char checksum = utility->getChecksumWithNoOffset(_current_wheel_data, EROBOT_WHEEL_DATA_PACKET_SIZE);

	buffer[startIndex]		= EROBOT_DEVICE_WHEEL_ID;
	buffer[startIndex+1]	= EROBOT_DEVICE_WHEEL_SUB_ID_1;
	buffer[startIndex+2]	= EROBOT_DEVICE_WHEEL_SUB_ID_2;
	buffer[startIndex+3]	= _current_wheel_command;
	buffer[startIndex+4]	= _current_wheel_data[0];
	buffer[startIndex+5]	= _current_wheel_data[1];
	buffer[startIndex+6]	= _current_wheel_data[2];
	buffer[startIndex+7]	= _current_wheel_data[3];
	buffer[startIndex+8]	= _current_wheel_data[4];
	buffer[startIndex+9]	= _current_wheel_data[5];
	buffer[startIndex+10]	= _current_wheel_data[6];
	buffer[startIndex+11]	= checksum;
}

void EWheelCore::addWheelStopCommandPacket(unsigned char* buffer, const int startIndex) {

	_current_wheel_command = EROBOT_COMMAND_WHEEL_EMERGENCY_STOP;
	buffer[startIndex]		= EROBOT_DEVICE_WHEEL_ID;
	buffer[startIndex+1]	= EROBOT_DEVICE_WHEEL_SUB_ID_1;
	buffer[startIndex+2]	= EROBOT_DEVICE_WHEEL_SUB_ID_2;
	buffer[startIndex+3]	= EROBOT_COMMAND_WHEEL_EMERGENCY_STOP;
	buffer[startIndex+4]	= 0x09;
}

void EWheelCore::addWheelSelfDiagnosisCommandPacket(unsigned char* buffer, const int startIndex) {

	buffer[startIndex]		= EROBOT_DEVICE_WHEEL_ID;
	buffer[startIndex+1]	= EROBOT_DEVICE_WHEEL_SUB_ID_1;
	buffer[startIndex+2]	= EROBOT_DEVICE_WHEEL_SUB_ID_2;
	buffer[startIndex+3]	= EROBOT_COMMAND_WHEEL_SELF_DIAGNOSIS;
	buffer[startIndex+4]	= 0x08;
}

void EWheelCore::addWheelSelfDiagnosisCommandPacket2(unsigned char* buffer, const int startIndex) {

	buffer[startIndex]		= EROBOT_DEVICE_WHEEL_ID;
	buffer[startIndex+1]	= EROBOT_DEVICE_WHEEL_SUB_ID_1;
	buffer[startIndex+2]	= EROBOT_DEVICE_WHEEL_SUB_ID_2;
	buffer[startIndex+3]	= 0x07;
	buffer[startIndex+4]	= 0x07;
}

int EWheelCore::parseAndSetWheelDataPacket(int dataPacketSize, unsigned char* dataPacket) {

	unsigned char checksum = ERobotFunctionFactory::getInstance()->getChecksum(dataPacket, dataPacketSize-1);

	/*
	 move by velocity xyt .... etc.... no response packet
	 so not check reponse packet
	 if(true) {
	 return RETURN_SUCCESS;
	 }
	 */
	if( (dataPacket[0] != 0x01)
		|| (dataPacket[1] != 0xF1)
		|| (dataPacket[2] != 0xF1)
		|| (dataPacket[3] != 0xF1) ) {
			//cout << "[EWheelCore] res packet header is different! " << endl;
			return RETURN_FAILED;
	}
	
	if (checksum != dataPacket[dataPacketSize-1]) {
		cout << "[EWheelCore] checksum check fail... " << endl;
		return RETURN_FAILED;
	}
	
	if( (dataPacket[4]!= 0x4F)					//   'O'
		|| (dataPacket[5]!= 0x4B) ) {			//   'K'
		return RETURN_FAILED;
	}
	return RETURN_SUCCESS;
}

int EWheelCore::parseAndSetWheelDataPacket(int dataPacketSize, unsigned char* dataPacket, const int startIndex) {

	// current wheel command no response 

	/*
	unsigned char checksum = ERobotFunctionFactory::getInstance()->getChecksumWithStartIndex(dataPacket, dataPacketSize-1,startIndex);
	
	if (checksum != dataPacket[startIndex+dataPacketSize-1]) {
		cout << "[EWheelCore] checksum check fail... " << endl;
		return RETURN_FAILED;
	}
	
	if( (dataPacket[4]!= 0x4F)					//   'O'
		|| (dataPacket[5]!= 0x4B) ) {			//   'K'
		return RETURN_FAILED;
	}
	*/
	
	return RETURN_SUCCESS;
}

void EWheelCore::setWheelValue(unsigned char command, int param1, int param2, int param3, int param4) {
//	printf("%x\n", command);
//	cout << "EWheelCore::setWheelValue : " << command << " , " << param1 << " , " << param2 << " , " << param3 << " , " << param4 << endl;
	_current_wheel_command = command;
	
	utility->intTo2Bytes(param1, _current_wheel_data);
	utility->intTo2Bytes(param2, _current_wheel_data+2);
	utility->intTo2Bytes(param3, _current_wheel_data+4);
	utility->intToByte(param4, _current_wheel_data+6);

	_current_wheel_param[0] = param1;
	_current_wheel_param[1] = param2;
	_current_wheel_param[2] = param3;
	_current_wheel_param[3] = param4;

//	printf("_current_wheel_command : %x\n", command);
//	for(int i=0; i<EROBOT_WHEEL_DATA_PACKET_SIZE ; i++) {
//		printf("%2x ", _current_wheel_data[i]);
//	}
//	cout << endl;
}

bool EWheelCore::isWheelPacket(unsigned char* buffer, const int startIndex) {

	if( (buffer[startIndex] != 0x01)
		|| (buffer[startIndex+1] != 0xF1)
		|| (buffer[startIndex+2] != 0xF1)
		|| (buffer[startIndex+3] != 0xF1) ) {
			return false;
	}
	return true;
}

bool EWheelCore::isWheelErrorPacket(unsigned char* buffer) {
	if( (buffer[0] != 0x01)
		|| (buffer[1] != 0xF1)
		|| (buffer[2] != 0xF1)
		|| (buffer[3] != 0xF8) ) {
			return false;
	}
	return true;
}

bool EWheelCore::isWheelStop() {
//#define	EROBOT_COMMAND_WHEEL_MOVE_BY_VELOCITY					0x00
//#define	EROBOT_COMMAND_WHEEL_MOVE_BY_VELOCITY_XYT				0x01
//#define	EROBOT_COMMAND_WHEEL_MOVE_TO_DELTA_XYT					0x02
//#define	EROBOT_COMMAND_WHEEL_MOVE_BY_TR_VELOCITY				0x04
//#define EROBOT_COMMAND_WHEEL_EMERGENCY_STOP						0x09
//#define EROBOT_COMMAND_WHEEL_SELF_DIAGNOSIS						0x08
//#define EROBOT_COMMAND_GE_VERSION								0x0F

//	printf("\n isWheelStop command %02x ", _current_wheel_command);

	if(_current_wheel_command == 0x00) {
//		cout << "_current_wheel_command == 0x00 " << endl;
		return true;
	} else if(_current_wheel_command == EROBOT_COMMAND_WHEEL_EMERGENCY_STOP) {
//		cout << "_current_wheel_command ==EROBOT_COMMAND_WHEEL_EMERGENCY_STOP " << endl;
		return true;
	} else if(_current_wheel_command == EROBOT_COMMAND_WHEEL_MOVE_BY_VELOCITY_XYT) {
//		cout << "_current_wheel_command ==EROBOT_COMMAND_WHEEL_MOVE_BY_VELOCITY_XYT " << endl;
		if(_current_wheel_param[0] == 0 && _current_wheel_param[1] == 0 && _current_wheel_param[2] == 0) {
//			cout << "_current_wheel_command ==EROBOT_COMMAND_WHEEL_MOVE_BY_VELOCITY_XYT 0 0 0 " << endl;
			return true;
		}
	} else if(_current_wheel_command == EROBOT_COMMAND_WHEEL_MOVE_BY_TR_VELOCITY) {
//		cout << "_current_wheel_command ==EROBOT_COMMAND_WHEEL_MOVE_BY_TR_VELOCITY " << endl;
		if(_current_wheel_param[0] == 0 && _current_wheel_param[1] == 0) {
//			cout << "_current_wheel_command ==EROBOT_COMMAND_WHEEL_MOVE_BY_TR_VELOCITY 0 0 " << endl;
			return true;
		}
	} else if(_current_wheel_command == EROBOT_COMMAND_WHEEL_MOVE_TO_DELTA_XYT) {
//		cout << "_current_wheel_command ==EROBOT_COMMAND_WHEEL_MOVE_TO_DELTA_XYT " << endl;
		return true;
	}
	return false;
}

int EWheelCore::parseAndSetWheelErrorDataPacket(int dataPacketSize, unsigned char* dataPacket, short errorCode[9]) {

	int nBuffHeaderOff = 4;
	int nByteLen  = 0;
	int nByteSize = 1;

	for(int i=0; i<9; i++) {
		errorCode[i] = ERobotFunctionFactory::getInstance()->byte2Int(dataPacket+nBuffHeaderOff+nByteLen,nByteSize);
		nByteLen += nByteSize;
//		cout << "parseAndSetWheelErrorDataPacket codeIndex " << i << " is " << errorCode[i] << endl;
	}
	return RETURN_SUCCESS;
}
