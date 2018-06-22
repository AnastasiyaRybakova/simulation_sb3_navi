#include "ERobotCore.h"
#include <time.h>
#include <ros/package.h>
#include <fstream>
#include <algorithm>
//#include <mmsystem.h>
#include <sstream>
#include <chrono>
#include <device/idif/gyro/GyroERobot.h>

using namespace std;
using namespace reactive::erobot::core;

ofstream battFile;
pthread_mutex_t ERobotCore::__comunicate_mutex =  PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
pthread_mutex_t ERobotCore::__body_mutex =  PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
pthread_mutex_t ERobotCore::__sensor_mutex =  PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
pthread_cond_t ERobotCore::__cv;
ERobotCore::ERobotCore() {

	battFile.open("/home/silbot3/robocare_ws/src/silbot3_core/batt_data.dat", ios::ate);
	//	m_nTimerID = timeSetEvent(1,0,TimeProc,(DWORD)0, TIME_PERIODIC);
	_ISEND = false;
	_isDeltaMove = false;
	_isLedUpdate = false;
	_isArmUpdate = false;
	_isPantiltUpdate = false;
	_isWheelUpdate = false;

	isTouchErrorChecking = false;
	isErrorCheckEnd = false;
	isGyroError = false;
	isUltraError = false;

	//	InitializeCriticalSection(&_comunicate);
	//	InitializeCriticalSection(&_body_comunicate);
	//	InitializeCriticalSection(&_sensor_comunicate);

	_baseConnection = new ERobotSerialConnector();
	_bodyConnection = new ERobotSerialConnector();
	_sensorConnection  = new ERobotSerialConnector();
	_etcConnection = new ERobotSerialConnector();


	ros::NodeHandle nh("~");

	nh.param<bool>("core/test", isTest, false);

	string baseComPort, bodyComPort, sensorComPort, etcComPort;
	int eBaudNumber;
	nh.param<std::string>("port/base", baseComPort, "ttyMP0");
	nh.param<std::string>("port/body", bodyComPort, "ttyMP1");
	nh.param<std::string>("port/sensor", sensorComPort, "ttyMP2");
	nh.param<std::string>("port/etc", etcComPort, "ttyMP3");

	_baseConnection->setup(baseComPort, EROBOT_SERIAL_BAUDRATE);
	_bodyConnection->setup(bodyComPort, EROBOT_SERIAL_BAUDRATE);
	_sensorConnection->setup(sensorComPort, EROBOT_SERIAL_BAUDRATE);
	_etcConnection->setup(etcComPort, EROBOT_SERIAL_BAUDRATE);

	_baseConnection->resetConnection();
	_bodyConnection->resetConnection();
	_sensorConnection->resetConnection();
	_etcConnection->resetConnection();

	_wheel_encoder_core = new EWheelEncoderCore();
	_ultrasonic_core	= new EUltraCore();
	_touch_core			= new ETouchCore();
	_wheel_core			= new EWheelCore();
	_arm_core			= new EArmCore();
	_pantilt_core		= new EPantiltCore();
	_led_core			= new ELEDCore();
	_battery_core		= new EBatteryCore();

	nh.param<std::string>("core/depth_sensor_type", depth_sensor_type, "kinect");

	nh.param<bool>("core/check_battery", isBatteryCheck, true);

	cout << "depth_sensor_type " << depth_sensor_type << endl;
	cout << "isBatteryCheck " << isBatteryCheck << endl;
	if(depth_sensor_type.compare("kinect") == 0 && isBatteryCheck) {
		ROS_INFO("\n\nSilbot3 VERSION IS 1.5 With Kinect 2\nwith Battery check\n");
		_body_core			= new EBodyCoreKinect();
	} else if(depth_sensor_type.compare("xtion") == 0 && isBatteryCheck) {
		ROS_INFO("\n\nSilbot3 VERSION IS 1.5 With Xtion\n \nwith Battery check\n");
		_body_core			= new EBodyCoreXtion();
	} else {
		cout << "\n\nSilbot3 VERSION IS not 1.5 \nwith Battery check\n" << endl;
	}

	helper = 0;
	_oldEncoderCount = 0;
	_deltaMoveValue = 0;

	this->_deltaPublisher = _nodehandle.advertise<silbot3_msgs::Device_Wheel_Msg>("/DeviceNode/Wheel/response", 1000);

	isTeaching = false;
	this->_teachingPublisher = _nodehandle.advertise<std_msgs::String>("/DeviceNode/Body/data", 1000);

}

ERobotCore::~ERobotCore() {
	delete _baseConnection;
	delete _bodyConnection;
	delete _sensorConnection;
	delete _etcConnection;

	delete _wheel_encoder_core;
	delete _ultrasonic_core;
	delete _touch_core;
	delete _wheel_core;
	delete _arm_core;
	delete _pantilt_core;
	delete _led_core;
	delete _battery_core;
}

ERobotCore* ERobotCore::getInstance() {
	static ERobotCore instance;
	return &instance;
}

void ERobotCore::checkMotorError() {
	/*	ROS_INFO("Start Silbot3 Device Motor Error Check");
	// arm check

	int serverDeviceType = CConfig::getInstance()->getParamValueInt("SERVER_DEVICE_TYPE");
	if(serverDeviceType == 6) {
		return;
	}
//	errorDetectingArm(short errorCode[6]);

	ROS_INFO("Start Arm Motor Error Check");
	int errorCount = 0;
	short armErrorCode[6] = {-1,-1,-1,-1,-1,-1};
	while(armErrorCode[0] == -1) {
		errorDetectingArm(armErrorCode);
		if(armErrorCode[0] == -1) {
			errorCount++;
			ros::Duration(0.2).sleep();
		}
		if(errorCount > 10) {
			ROS_WARN("Body Serial Connection Error occured");
			boost::thread( &ERobotCore::viewMessageBox, this, "Body Serial Connection Error have Some Error. Please Check Serial Connection", "Body Serial Connection Error" );
			break;
		}
	}

	if(errorCount <= 10) {
		bool isArmError = false;
		for(int i=0; i<6; i++) {
			if(armErrorCode[i] != 0) {
				isArmError = true;
			}
		}
		if(isArmError) {
			ROS_WARN("Arm Motors have Some Error. Please Run Self Diagnosis Application");
			boost::thread( &ERobotCore::viewMessageBox, this, "Arm Motors have Some Error. Please Run Self Diagnosis Application", "Arm Motors Error Check");
		} else {
			ROS_INFO("Arm Motors have No Error");
		}
	}

	// pantilt check
//	void errorDetectingPantilt(short errorCode[3]);
	ROS_INFO("Start Pantilt Motor Error Check");
	errorCount = 0;
	short pantiltErrorCode[3] = {-1,-1,-1};
	while(pantiltErrorCode[0] == -1) {
		errorDetectingPantilt(pantiltErrorCode);
		ROS_INFO("pantiltErrorCode[0] : %d", pantiltErrorCode[0]);
		if(pantiltErrorCode[0] == -1) {
			ros::Duration(0.2).sleep();
			errorCount++;
		}
		if(errorCount > 10) {
			ROS_WARN("Body Serial Connection Error occured");
			boost::thread( &ERobotCore::viewMessageBox, this, "Body Serial Connection Error have Some Error. Please Check Serial Connection", "Body Serial Connection Error");
			break;
		}
	}
	if(errorCount <= 10) {
		bool isPantiltError = false;
		for(int i=0; i<3; i++) {
			if(pantiltErrorCode[i] != 0) {
				isPantiltError = true;
			}
		}
		if(isPantiltError) {
			ROS_WARN("Pantilt Motors have Some Error. Please Run Self Diagnosis Application");
			boost::thread( &ERobotCore::viewMessageBox, this, "Pantilt Motors have Some Error. Please Run Self Diagnosis Application", "Pantilt Motors Error Check");
		} else {
			ROS_INFO("Pantilt Motors have No Error");
		}
	}

	isUltraError = true;
	isGyroError = true;

	isErrorCheckEnd = false;

	bool isWheelError = false;
	// wheel check
	short wheelErrorCode[9] = {-1,-1,-1,-1,-1,-1,-1,-1,-1};

	int bootCheckType = CConfig::getInstance()->getParamValueInt("CHECK_MOTOR_TYPE");
	if(bootCheckType != 1) {
		selfDiagnosisWheelNoMove(wheelErrorCode);
	} else {
//		MessageBox( 0, "Robot will start moving. Please disconnect charge and keep away.", "Robot Move Start", 0 );
		boost::thread( &ERobotCore::_checkUltrasonicGyroError, this );
		ROS_INFO("Start Wheel Motor Move and Error Check");
		selfDiagnosfisWheel(wheelErrorCode);
	}


	isErrorCheckEnd = true;

	if(wheelErrorCode[0] == -1) {
		ROS_WARN("Mobile Base Serial Connection Error occured");
		boost::thread( &ERobotCore::viewMessageBox, this, "Mobile Base Serial Connection Error have Some Error. Please Check Serial Connection", "Mobile Base Serial Connection Error");
	} else {

		if(wheelErrorCode[0] != 0) {
			isWheelError = true;
		}

		if(isWheelError) {
			ROS_WARN("Wheel Motors have Some Error. Please Run Self Diagnosis Application");
			boost::thread( &ERobotCore::viewMessageBox, this, "Wheel Motors have Some Error. Please Run Self Diagnosis Application", "Wheel Motors Error Check");
		} else {
			ROS_INFO("Wheel Motors have No Error");
		}
	}
	 */
}

void ERobotCore::_checkWheelErrorTimeover() {
	/*
	ros::Duration(30).sleep();
	if(!isErrorCheckEnd) {
		isErrorCheckEnd = true;
		_baseConnection->resetConnection();
	}
	 */
}

void ERobotCore::_checkUltrasonicGyroError() {
	ROS_INFO("Start Ultrasonic Gyro Sensor Error Check");
	CGyroERobot::getInstance();
	ros::Duration(3).sleep();
	bool ultraChangeCheckArray[8];
	bool gyroChangeCheck = false;

	unsigned short ultraData[8];
	unsigned short oldUltraData[8];
	for(int i=0; i<8; i++) {
		ultraChangeCheckArray[i] = false;
		ultraData[i] = -999;
		oldUltraData[i] = -999;
	}

	int readPacketSize				= 13;  // ultra
	int sendPacketSize				= 5;

	unsigned char* sendPacket		= new unsigned char[sendPacketSize];
	unsigned char* receivePacket	= new unsigned char[readPacketSize];

	CGyroData gyroData(0,0);

	double gyroAngle, oldGyroAngle;
	oldGyroAngle = -999.99;
	while(!isErrorCheckEnd) {
		memset(sendPacket, 0x00, sendPacketSize);
		_ultrasonic_core->addUltrsonicCommandPacket( EROBOT_COMMAND_ULTRASONIC_GET_DATA, sendPacket, 0);
		_sensorConnection->writeBytes( sendPacket, sendPacketSize );

		int readSize = _sensorConnection->readBytes(receivePacket,readPacketSize);
		if( readSize < readPacketSize) {
			ros::Duration(0.1).sleep();
			continue;
		}
		if(_ultrasonic_core->isUltrasonicPacket(receivePacket,0)) {
			int ultraReadPacketSize	= ERobotFunctionFactory::getInstance()->getReadPacketSize(EROBOT_TYPE_ULTRASONIC);
			if(RETURN_SUCCESS == _ultrasonic_core->parseAndSetUltrsonicDataPacket(ultraReadPacketSize, receivePacket, 0)) {

			}
		} else {
			//			cout << "fail : [_updateSensor] broken or checksum error" << endl;
			_sensorConnection->resetConnection();
			ros::Duration(0.1).sleep();
		}

		_ultrasonic_core->getUltrasonicValue(ultraData, 8);
		if(oldUltraData[0] != -999) {
			for(int i=0; i<8; i++) {
				if(oldUltraData[i] != ultraData[i]) {
					ultraChangeCheckArray[i] = true;
				}
			}

			bool allClear = true;
			for(int i=0; i<8; i++) {
				if(!ultraChangeCheckArray[i]) {
					allClear = false;
				}
			}
			if(allClear) {
				isUltraError = false;
			}

		} else {
			for(int i=0; i<8; i++) {
				oldUltraData[i] = ultraData[i];
			}
		}

		gyroData = CGyroERobot::getInstance()->readData();
		gyroAngle = gyroData.getAngle();
		if(oldGyroAngle != -999.99) {
			if(oldGyroAngle - gyroAngle < -0.5 || oldGyroAngle - gyroAngle > 0.5) {
				isGyroError = false;
			}
			oldGyroAngle = gyroAngle;
		} else {
			oldGyroAngle = gyroAngle;
		}

		if(!isGyroError && !isUltraError) {
			break;
		}
		ros::Duration(0.1).sleep();
	}

	if(isUltraError) {
		ROS_WARN("Ultrasonic Sensors have Some Error. Please Run Self Diagnosis Application");
		viewMessageBox("Ultrasonic Sensors have Some Error. Please Run Self Diagnosis Application", "Ultrasonic Sensors Error Check");
	} else {
		ROS_INFO("Ultrasonic Sensors have No Error");
	}
	if(isGyroError) {
		ROS_WARN("Gyro Sensor has Some Error. Please Run Self Diagnosis Application");
		viewMessageBox("Gyro Sensor have Some Error. Please Run Self Diagnosis Application", "Gyro Sensor Error Check");
	} else {
		ROS_INFO("Gyro Sensor has No Error");
	}
}

void ERobotCore::viewMessageBox(const char* message, const char* title) {
	//	MessageBox( 0, message, title, 0 );
}


void ERobotCore::checkVersion() {
	//	CreateDirectory ("C:\\opt\\cirdeps\\data\\Device", NULL);


	string pkgpath = ros::package::getPath("silbot3_core");
	stringstream ss;
	ss<<pkgpath<<"/config/device_version.dat";

	ofstream versionFile;

	versionFile.open(ss.str().c_str());

	versionFile << "Wheel\t\t:\t" << checkVersionWheel() << endl;
	versionFile << "Body\t\t:\t" << checkVersionArm() << endl;
	//versionFile << "Head\t\t:\t" << checkVersionHead() << endl;
	versionFile << "Ultrasonic\t:\t" << checkVersionUltra() << endl;
	versionFile << "Etc\t\t:\t" << checkVersionSensors() << endl;

	cout << "Wheel\t\t:\t" << checkVersionWheel() << endl;
	cout << "Body\t\t:\t" << checkVersionArm() << endl;
	//cout << "Head\t\t:\t" << checkVersionHead() << endl;
	cout << "Ultrasonic\t:\t" << checkVersionUltra() << endl;
	cout << "Etc\t\t:\t" << checkVersionSensors() << endl;

	versionFile.close();
}


string ERobotCore::checkVersionWheel() {
	//	Mobile	03 F3 F3 0F 0F
	//	(5bytes)	01 F1 F1 FF 57 48 45 45 4C 30 30 30 30 CS
	//	(14Bytes)	ID+Header+WHEEL0000
	//	(0000 : VERSION)

	ROS_DEBUG("check version wheel");
	int checkVersionSize = 5;
	unsigned char* checkVersionPacket = new unsigned char[checkVersionSize];
	checkVersionPacket[0] = 0x03;
	checkVersionPacket[1] = 0xF3;
	checkVersionPacket[2] = 0xF3;
	checkVersionPacket[3] = 0x0F;
	checkVersionPacket[4] = 0x0F;
	ROS_DEBUG("send check version wheel packet");
	_baseConnection->writeBytes(checkVersionPacket,checkVersionSize);

	int receivePacketSize = 14;
	unsigned char* receivePacket	= new unsigned char[receivePacketSize];
	memset(receivePacket, 0x00, receivePacketSize);

	int readSize = 0;
	int count = 0;
	ROS_DEBUG("read check version wheel packet");
	while(readSize < receivePacketSize && count <= 10) {
		readSize = _baseConnection->readBytes(receivePacket,receivePacketSize);
		if(readSize < receivePacketSize ) {
			ros::Duration(0.1).sleep();
		}
		count++;
	}
	stringstream ss;
	ss << receivePacket[9] << "."   << receivePacket[10] << "."  << receivePacket[11] << "."  << receivePacket[12];

	string result;
	ss >> result;
	ROS_DEBUG("check version wheel result : %s",result.c_str());
	return result;
}
string ERobotCore::checkVersionArm() {
	//	��	08 F8 F8 0F 0F
	//	(5Bytes)	01 F8 F8 FF 5F 5F 5F 5F 41 52 4D 30 30 30 30 CS
	//	(16Bytes)	ID+Header+____ARM0000
	//	(0000 : VERSION)

	ROS_DEBUG("check version arm");
	int checkVersionSize = 5;
	unsigned char* checkVersionPacket = new unsigned char[checkVersionSize];
	checkVersionPacket[0] = 0x08;
	checkVersionPacket[1] = 0xF8;
	checkVersionPacket[2] = 0xF8;
	checkVersionPacket[3] = 0x0F;
	checkVersionPacket[4] = 0x0F;
	ROS_DEBUG("send check version arm packet");
	_bodyConnection->writeBytes(checkVersionPacket,checkVersionSize);

	int receivePacketSize = 16;
	unsigned char* receivePacket	= new unsigned char[receivePacketSize];
	memset(receivePacket, 0x00, receivePacketSize);

	int readSize = 0;
	int count = 0;
	ROS_DEBUG("read check version arm packet");
	while(readSize < receivePacketSize && count <= 10) {
		readSize = _bodyConnection->readBytes(receivePacket,receivePacketSize);
		if(readSize < receivePacketSize ) {
			ros::Duration(0.1).sleep();
		}
		count++;
	}


	stringstream ss;
	ss << receivePacket[11] << "."   << receivePacket[12] << "."  << receivePacket[13] << "."  << receivePacket[14];

	string result;
	ss >> result;
	ROS_DEBUG("check version arm result : %s",result.c_str());
	return result;
}
string ERobotCore::checkVersionHead() {
	//	�Ӹ�	07 F7 F7 0F 0F
	//	(5Bytes)	01 F7 F7 FF 5F 48 45 41 44 30 30 30 30 CS
	//	(14Bytes)	ID+Header+_HEAD0000
	//	(0000 : VERSION)
	ROS_DEBUG("check version head");
	int checkVersionSize = 5;
	unsigned char* checkVersionPacket = new unsigned char[checkVersionSize];
	checkVersionPacket[0] = 0x07;
	checkVersionPacket[1] = 0xF7;
	checkVersionPacket[2] = 0xF7;
	checkVersionPacket[3] = 0x0F;
	checkVersionPacket[4] = 0x0F;
	ROS_DEBUG("send check version head packet");
	_bodyConnection->writeBytes(checkVersionPacket,checkVersionSize);

	int receivePacketSize = 14;
	unsigned char* receivePacket	= new unsigned char[receivePacketSize];
	memset(receivePacket, 0x00, receivePacketSize);

	int readSize = 0;
	int count = 0;
	ROS_DEBUG("read check version head packet");
	while(readSize < receivePacketSize && count <= 10) {
		readSize = _bodyConnection->readBytes(receivePacket,receivePacketSize);
		if(readSize < receivePacketSize ) {
			ros::Duration(0.1).sleep();
		}
		count++;
	}
	stringstream ss;
	ss << receivePacket[9] << "."   << receivePacket[10] << "."  << receivePacket[11] << "."  << receivePacket[12];

	string result;
	ss >> result;
	ROS_DEBUG("check version head result : %s",result.c_str());
	return result;
}
string ERobotCore::checkVersionUltra() {
	//	������	05 F5 F5 0F 0F
	//	(5Bytes)	01 F2 F2 FF 53 4F 4E 41 52 30 30 30 30 CS
	//	(14Bytes)	ID+Header+SONAR0000
	//	(0000 : VERSION)

	ROS_DEBUG("check version ultra");
	int checkVersionSize = 5;
	unsigned char* checkVersionPacket = new unsigned char[checkVersionSize];
	checkVersionPacket[0] = 0x05;
	checkVersionPacket[1] = 0xF5;
	checkVersionPacket[2] = 0xF5;
	checkVersionPacket[3] = 0x0F;
	checkVersionPacket[4] = 0x0F;
	ROS_DEBUG("send check version ultra packet");
	_sensorConnection->writeBytes(checkVersionPacket,checkVersionSize);

	int receivePacketSize = 14;
	unsigned char* receivePacket	= new unsigned char[receivePacketSize];
	memset(receivePacket, 0x00, receivePacketSize);

	int readSize = 0;
	int count = 0;
	ROS_DEBUG("read check version ultra packet");
	while(readSize < receivePacketSize && count <= 10) {
		readSize = _sensorConnection->readBytes(receivePacket,receivePacketSize);
		if(readSize < receivePacketSize ) {
			ros::Duration(0.1).sleep();
		}
		count++;
	}
	stringstream ss;
	ss << receivePacket[9] << "."   << receivePacket[10] << "."  << receivePacket[11] << "."  << receivePacket[12];

	string result;
	ss >> result;
	ROS_DEBUG("check version ultra result : %s",result.c_str());
	return result;
}
string ERobotCore::checkVersionSensors() {
	//	LED & Touch & battery	06 F6 F6 0F 0F
	//	(5Bytes)	01 F1 F1 FF 5F 5F 5F 4C 45 44 30 30 30 30 CS
	//	(15Btyes)	ID+Header+___LED0000
	//	(0000 : VERSION)

	ROS_DEBUG("check version etc");
	int checkVersionSize = 5;
	unsigned char* checkVersionPacket = new unsigned char[checkVersionSize];
	checkVersionPacket[0] = 0x06;
	checkVersionPacket[1] = 0xF6;
	checkVersionPacket[2] = 0xF6;
	checkVersionPacket[3] = 0x0F;
	checkVersionPacket[4] = 0x0F;
	ROS_DEBUG("send check version etc packet");
	_etcConnection->writeBytes(checkVersionPacket,checkVersionSize);

	int receivePacketSize = 15;
	unsigned char* receivePacket	= new unsigned char[receivePacketSize];
	memset(receivePacket, 0x00, receivePacketSize);

	int readSize = 0;
	int count = 0;
	ROS_DEBUG("read check version etc packet");
	while(readSize < receivePacketSize && count <= 10) {
		readSize = _etcConnection->readBytes(receivePacket,receivePacketSize);
		if(readSize < receivePacketSize ) {
			ros::Duration(0.1).sleep();
		}
		count++;
	}
	stringstream ss;
	ss << receivePacket[10] << "."   << receivePacket[11] << "."  << receivePacket[12] << "."  << receivePacket[13];

	string result;
	ss >> result;
	ROS_DEBUG("check version etc result : %s",result.c_str());
	return result;
}

void ERobotCore::initERobotCore() {

	ROS_INFO("init Erobot core start");

	boost::thread( &ERobotCore::_updateBase, this );
	boost::thread( &ERobotCore::_updateSensor, this );
	boost::thread( &ERobotCore::_updateEtc, this );
	boost::thread( &ERobotCore::_updateBody, this );
	boost::thread( &ERobotCore::_updateWheelEncoder, this );
	boost::thread( &ERobotCore::_sendBaseResponse, this );

	ros::NodeHandle nh("~");

	bool checkMotor;
	nh.param<bool>("core/check_motor_error", checkMotor, false);

	if(checkMotor) {
	 	boost::thread( &ERobotCore::_updateMotorError, this );
	}

	ROS_INFO("init Erobot core finish!");
	//	boost::thread( &ERobotCore::_updateBodyPosition, this );
}

void ERobotCore::_updateMotorError() {
	ROS_INFO("Start Silbot3 Device Update Motor Error");
	// arm check

	ros::NodeHandle nh("~");

	double updatePeriod;
	nh.param<double>("core/check_motor_error_preriod", updatePeriod, 60.0);

	ROS_INFO("UPDATE_ERROR_CHECK_PERIOD : %f", updatePeriod);

	while(!_ISEND && !isTest /* mock mode*/) {
		ros::Duration(updatePeriod).sleep();

		if(!_isArmUpdate && !_isPantiltUpdate) {
			ROS_DEBUG("Start Arm Motor Error Check");
			int errorCount = 0;
			short armErrorCode[6] = {-1,-1,-1,-1,-1,-1};
			while(armErrorCode[0] == -1) {
				errorDetectingArm(armErrorCode);
				if(armErrorCode[0] == -1) {
					errorCount++;
					ros::Duration(0.2).sleep();
				}
				if(errorCount > 10) {
					ROS_WARN("Body Serial Connection Error occured");
					boost::thread( &ERobotCore::viewMessageBox, this, "Body Serial Connection Error have Some Error. Please Check Serial Connection", "Body Serial Connection Error");
					//					MessageBox( 0, "Body Serial Connection Error have Some Error. Please Check Serial Connection", "Body Serial Connection Error", 0 );
					break;
				}
			}

			if(errorCount <= 10) {
				bool isArmError = false;
				for(int i=0; i<6; i++) {
					if(armErrorCode[i] != 0) {
						isArmError = true;
					}
				}
				if(isArmError) {
					ROS_WARN("Arm Motors have Some Error. Please Run Self Diagnosis Application");
					boost::thread( &ERobotCore::viewMessageBox, this, "Arm Motors have Some Error. Please Run Self Diagnosis Application", "Arm Motors Error Check");
					//					MessageBox( 0, "Arm Motors have Some Error. Please Run Self Diagnosis Application", "Arm Motors Error Check", 0 );
				} else {
					ROS_DEBUG("Arm Motors have No Error");
				}
			}

			// pantilt check
			//	void errorDetectingPantilt(short errorCode[3]);
			ROS_DEBUG("Start Pantilt Motor Error Check");
			errorCount = 0;
			short pantiltErrorCode[3] = {-1,-1,-1};
			while(pantiltErrorCode[0] == -1) {
				errorDetectingPantilt(pantiltErrorCode);
				//				ROS_INFO("pantiltErrorCode[0] : %d", pantiltErrorCode[0]);
				if(pantiltErrorCode[0] == -1) {
					ros::Duration(0.2).sleep();
					errorCount++;
				}
				if(errorCount > 10) {
					ROS_WARN("Body Serial Connection Error occured");
					boost::thread( &ERobotCore::viewMessageBox, this, "Body Serial Connection Error have Some Error. Please Check Serial Connection", "Body Serial Connection Error");
					//					MessageBox( 0, "Body Serial Connection Error have Some Error. Please Check Serial Connection", "Body Serial Connection Error", 0 );
					break;
				}
			}
			if(errorCount <= 10) {
				bool isPantiltError = false;
				for(int i=0; i<3; i++) {
					if(pantiltErrorCode[i] != 0) {
						isPantiltError = true;
					}
				}
				if(isPantiltError) {
					ROS_WARN("Pantilt Motors have Some Error. Please Run Self Diagnosis Application");
					boost::thread( &ERobotCore::viewMessageBox, this, "Pantilt Motors have Some Error. Please Run Self Diagnosis Application", "Pantilt Motors Error Check");
					//					MessageBox( 0, "Pantilt Motors have Some Error. Please Run Self Diagnosis Application", "Pantilt Motors Error Check", 0 );
				} else {
					ROS_DEBUG("Pantilt Motors have No Error");
				}
			}
		}

		bool isWheelError = false;
		// wheel check
		short wheelErrorCode[9] = {-1,-1,-1,-1,-1,-1,-1,-1,-1};

		//	void selfDiagnosisWheel(short errorCode[9]);
		//		boost::thread( &ERobotCore::_checkUltrasonicGyroError, this );
		selfDiagnosisWheelNoMove(wheelErrorCode);

		if(wheelErrorCode[0] == -1) {
			ROS_WARN("Mobile Base Serial Connection Error occured");
			boost::thread( &ERobotCore::viewMessageBox, this, "Mobile Base Serial Connection Error have Some Error. Please Check Serial Connection", "Mobile Base Serial Connection Error");
		} else {

			if(wheelErrorCode[0] != 0) {
				isWheelError = true;
			}

			if(isWheelError) {
				ROS_WARN("Wheel Motors have Some Error. Please Run Self Diagnosis Application");
				boost::thread( &ERobotCore::viewMessageBox, this, "Wheel Motors have Some Error. Please Run Self Diagnosis Application", "Wheel Motors Error Check");
			} else {
				ROS_DEBUG("Wheel Motors have No Error");
			}
		}

	}

}

void ERobotCore::_updateBase() {
	ros::Duration(2).sleep();
	ROS_INFO("ERobotCore::_updateBase start");
	int state			= RETURN_FAILED;
	int indexPoint		= 0;
	int successCount	= 0;

	int readPacketSize				= 14+9;  // wheel encoder
	int sendCommandPacketSize		= 12;
	int stopPacketSize				= 5;
	int deltaPacketSize				= 7;

	unsigned char command, oldCommand;

	unsigned char* sendCommandPacket 	= new unsigned char[sendCommandPacketSize];
	unsigned char* stopPacket 			= new unsigned char[stopPacketSize];
	unsigned char* deltaResponsePacket 	= new unsigned char[deltaPacketSize];

	pthread_mutex_lock(&__comunicate_mutex);
	helper = 1;
	pthread_cond_signal(&__cv);
	pthread_mutex_unlock(&__comunicate_mutex);
	bool isFirst = true;

	memset(stopPacket, 0x00, stopPacketSize);
	_wheel_core->addWheelStopCommandPacket(stopPacket, 0);

	ros::Duration commandDelay = ros::Duration(0.1);
	ros::Rate baseUpdateRate = 10;
	ros::Duration stopDelay = ros::Duration(0.5);
	_isWheelUpdate = false;
	while(!_ISEND && !isTest ) {
		baseUpdateRate.sleep();
		if(_isWheelUpdate) {
			_isWheelUpdate = false;
			_wheel_core->getWheelCommandPacket(sendCommandPacket);
			pthread_mutex_lock(&__comunicate_mutex);
			command = sendCommandPacket[3];
			if(_isDeltaMove) {
				_isDeltaMove = false;

				_baseConnection->writeBytes(stopPacket,stopPacketSize);
				baseUpdateRate.sleep();
				_baseConnection->writeBytes(stopPacket,stopPacketSize);
				stopDelay.sleep();

				_deltaMoveValue = -1;
			}

			if(command == EROBOT_COMMAND_WHEEL_MOVE_TO_DELTA_XYT) {
				ROS_DEBUG("Delta move started\n");
				_isDeltaMove = true;
				_deltaMoveValue = -99;
				_baseConnection->writeBytes(sendCommandPacket,sendCommandPacketSize);
			} else {
				_deltaMoveValue = 1;
				_isDeltaMove = false;
				_baseConnection->writeBytes(sendCommandPacket,sendCommandPacketSize);
			}
			//			printf("send \t");
			//			for(int i=0 ; i<sendCommandPacketSize ; i++) {
			//				printf("%02x ", sendCommandPacket[i]);
			//			}
			//			printf("\n");

			pthread_mutex_unlock(&__comunicate_mutex);
		}
	}

	delete[] sendCommandPacket;
	delete[] stopPacket;
	delete[] deltaResponsePacket;

}
//
//void ERobotCore::_updateWheelEncoder() {
//	ros::Duration(2).sleep();
//	ROS_INFO("ERobotCore::_updateWheelEncoder start");
//	int state				= RETURN_FAILED;
//	int indexPoint		= 0;
//	int successCount		= 0;
//	int sendPacketSize				= 5;  // wheel encoder
//	int encoderPacketSize= 14+9;
//	//	int encoderPacketSize			= 5;
//	int deltaPacketSize	= 7;
//
//	CConfig* config = CConfig::getInstance();
//	int serverDeviceType = config->getParamValueInt("SERVER_DEVICE_TYPE");
//
//	unsigned char* oldEncoderPacket	= new unsigned char[encoderPacketSize];
//	unsigned char* deltaResponsePacket = new unsigned char[deltaPacketSize];
//	unsigned char* receivePacket = new unsigned char[encoderPacketSize];
//
//
//	unsigned char* sendPacket		= new unsigned char[sendPacketSize];
//	memset(sendPacket, 0x00, sendPacketSize);
//	sendPacket[0] = 0x03;
//	sendPacket[1] = 0xf3;
//	sendPacket[2] = 0xf3;
//	sendPacket[3] = 0x0e;
//	sendPacket[4] = 0x0e;
//
//	stringstream responseStream;
//	responseStream.str("");
//	ros::Rate encoderRate = 7;
//	ros::Duration sec1 = ros::Duration(1);
//
//	bool isFirst = true;
//
//	while(!_ISEND && serverDeviceType != 6 ) {
//
//		encoderRate.sleep();
//		//		EnterCriticalSection(&_comunicate);
//		pthread_mutex_lock(&__comunicate_mutex);
//
//		_baseConnection->writeBytes( sendPacket, sendPacketSize );
//		int readRealSize = _baseConnection->readBytes(receivePacket,encoderPacketSize);
//		ROS_DEBUG("Wheel encoder read real size : %d " , readRealSize);
//		//				printf("read size : %d \t",readRealSize);
//		//				for(int i=0 ; i<readRealSize ; i++) {
//		//					printf("%02x ", receivePacket[i]);
//		//				}
//		//				printf("\n");
//
//		if(readRealSize == 0) {
//			ROS_INFO("Wheel encoder read real size 000");
//			_baseConnection->resetConnection();
//			sec1.sleep();
//		} else if(readRealSize < deltaPacketSize) {
//			//			f1 f1 4f 4b 04
//			//			char* buff = new char[10];
//			//			for(int i=0 ; i<readRealSize ; i++) {
//			//				sprintf(buff, "%02x ", receivePacket[i]);
//			//				responseStream << buff;
//			//			}
//			//			delete buff;
//			//			responseStream.str("");
//			_baseConnection->resetConnection();
//			sec1.sleep();
//		} else if(readRealSize == deltaPacketSize) {
//			//			01 f1 f1 f1 4f 4b 04
//			ROS_DEBUG("Delta move finished...\n");
//
//			bool isTrue = true;
//			if (receivePacket[0] != 0x01) {
//				isTrue = false;
//			} else if (receivePacket[1] != 0xF1) {
//				isTrue = false;
//			} else if (receivePacket[2] != 0xF1) {
//				isTrue = false;
//			} else if (receivePacket[3] != 0xF1) {
//				isTrue = false;
//			} else if (receivePacket[4] != 0x4F) {
//				isTrue = false;
//			} else if (receivePacket[5] != 0x4b) {
//				isTrue = false;
//			} else if (receivePacket[6] != 0x04) {
//				isTrue = false;
//			}
//			if(isTrue) {
//				//				char* buff = new char[10];
//				//				for(int i=0 ; i<readRealSize ; i++) {
//				//					sprintf(buff, "%02x ", receivePacket[i]);
//				//					responseStream << buff;
//				//				}
//				//				delete buff;
//				//				responseStream.str("");
//				_isDeltaMove = false;
//				_deltaMoveValue = 1;
//			}
//		} else if(readRealSize < encoderPacketSize) {
//			//			char* buff = new char[10];
//			//			for(int i=0 ; i<readRealSize ; i++) {
//			//				sprintf(buff, "%02x ", receivePacket[i]);
//			//				responseStream << buff;
//			//			}
//			//			delete buff;
//			//			responseStream.str("");
//
//			bool isTrue = true;
//			if(!isTrue) {
//				isTrue = true;
//				if (receivePacket[readRealSize-7] != 0x01) {
//					isTrue = false;
//				} else if (receivePacket[readRealSize-6] != 0xF1) {
//					isTrue = false;
//				} else if (receivePacket[readRealSize-5] != 0xF1) {
//					isTrue = false;
//				} else if (receivePacket[readRealSize-4] != 0xF1) {
//					isTrue = false;
//				} else if (receivePacket[readRealSize-3] != 0x4F) {
//					isTrue = false;
//				} else if (receivePacket[readRealSize-2] != 0x4b) {
//					isTrue = false;
//				} else if (receivePacket[readRealSize-1] != 0x04) {
//					isTrue = false;
//				}
//
//				if(isTrue) {
//					_isDeltaMove = false;
//					_deltaMoveValue = 1;
//				}
//			}
//			if(!isTrue) {
//				isTrue = true;
//				if (receivePacket[0] != 0x01) {
//					isTrue = false;
//				} else if (receivePacket[1] != 0xF1) {
//					isTrue = false;
//				} else if (receivePacket[2] != 0xF1) {
//					isTrue = false;
//				} else if (receivePacket[3] != 0xF1) {
//					isTrue = false;
//				} else if (receivePacket[4] != 0x4F) {
//					isTrue = false;
//				} else if (receivePacket[5] != 0x4b) {
//					isTrue = false;
//				} else if (receivePacket[6] != 0x04) {
//					isTrue = false;
//				}
//
//				if(isTrue) {
//					_isDeltaMove = false;
//					_deltaMoveValue = 1;
//				}
//			}
//			_isDeltaMove = false;
//			_deltaMoveValue = 1;
//			_baseConnection->resetConnection();
//			encoderRate.sleep();
//		} else if(readRealSize == encoderPacketSize) {
//			//			char* buff = new char[10];
//			//			for(int i=0 ; i<readRealSize ; i++) {
//			//				sprintf(buff, "%02x ", receivePacket[i]);
//			//				responseStream << buff;
//			//			}
//			//			cout << responseStream.str() << endl;
//			//			delete buff;
//			//			LOG4CXX_DEBUG(wheel_logger, "correct encoder Packet : " << readRealSize << " : " << responseStream.str());
//			//			responseStream.str("");
//
//			bool isTrue = true;
//
//
//			//			read size : 23 	01 f1 f1 f1 4f 4b 04 01 f1 f1 f1 4f 4b 04 01 f1 f1 f4 4f 4b 04 45 4c
//			//			read size : 23 	30 31 31 36 00 0d 74 05 d5 df 00 00 86 f0 01 f1 f1 f4 4f 4b 04 45 4c
//
//			for(int st = 0; st < 23 - 7; st++) {
//				if (receivePacket[st+0] != 0x01) {
//					isTrue = false;
//				} else if (receivePacket[st+1] != 0xF1) {
//					isTrue = false;
//				} else if (receivePacket[st+2] != 0xF1) {
//					isTrue = false;
//				} else if (receivePacket[st+3] != 0xF1) {
//					isTrue = false;
//				} else if (receivePacket[st+4] != 0x4F) {
//					isTrue = false;
//				} else if (receivePacket[st+5] != 0x4b) {
//					isTrue = false;
//				} else if (receivePacket[st+6] != 0x04) {
//					isTrue = false;
//				}
//				if(isTrue) {
//					break;
//				}
//			}
//			if(isTrue) {
//				//				char* buff = new char[10];
//				//				for(int i=0 ; i<readRealSize ; i++) {
//				//					sprintf(buff, "%02x ", receivePacket[i]);
//				//					responseStream << buff;
//				//				}
//				//				delete buff;
//				//				responseStream.str("");
//				_isDeltaMove = false;
//				_deltaMoveValue = 1;
//				_baseConnection->resetConnection();
//				encoderRate.sleep();
//			} else {
//				indexPoint = 0;
//				if(_isDeltaMove) {
//					bool isSame = true;
//					for(int ind = 0; ind < encoderPacketSize -1; ind++) {
//						if(receivePacket[ind] != oldEncoderPacket[ind]) {
//							isSame = false;
//						}
//					}
//
//					if(isSame) {
//						_oldEncoderCount++;
//					} else {
//						_oldEncoderCount = 0;
//						for(int ind = 0; ind < encoderPacketSize; ind++) {
//							oldEncoderPacket[ind] = receivePacket[ind];
//						}
//					}
//					if(_oldEncoderCount > 10) {
//						_isDeltaMove = false;
//						_deltaMoveValue = 1;
//						_oldEncoderCount = 0;
//					}
//				}
//
//
//				if(isFirst) {
//					isFirst = false;
//					_wheel_encoder_core->parseAndSetWheelEncoderInitDataPacket(encoderPacketSize,receivePacket,indexPoint);
//				}
//				if(_wheel_encoder_core->isWheelEncoderPacket(receivePacket,indexPoint)) {
//					if(RETURN_SUCCESS == _wheel_encoder_core->parseAndSetWheelEncoderDataPacket(encoderPacketSize,receivePacket,indexPoint)) {
//
//					}
//				} else {
//					ROS_ERROR("\nfail : [_updateBase] broken or checksum error\n");
//					_baseConnection->resetConnection();
//					encoderRate.sleep();
//				}
//			}
//
//
//		}
//
//		//		LeaveCriticalSection(&_comunicate);
//		pthread_mutex_unlock(&__comunicate_mutex);
//	}
//
//	delete[] sendPacket;
//	delete[] receivePacket;
//	delete[] deltaResponsePacket;
//	delete[] oldEncoderPacket;
//}


void ERobotCore::_updateWheelEncoder() {
	ros::Duration(2).sleep();
	ROS_INFO("ERobotCore::_updateWheelEncoder start");
	int state			= RETURN_FAILED;
	int indexPoint		= 0;
	int successCount	= 0;
	int sendPacketSize				= 5;  // wheel encoder
	int encoderPacketSize			= 14+9;
//	int encoderPacketSize			= 5;
	int deltaPacketSize				= 7;

	// CConfig* config = CConfig::getInstance();
	// int serverDeviceType = config->getParamValueInt("SERVER_DEVICE_TYPE");

	unsigned char* oldEncoderPacket	= new unsigned char[encoderPacketSize];
	unsigned char* deltaResponsePacket = new unsigned char[deltaPacketSize];
	unsigned char* receivePacket = new unsigned char[encoderPacketSize];


	unsigned char* sendPacket		= new unsigned char[sendPacketSize];
	memset(sendPacket, 0x00, sendPacketSize);
	sendPacket[0] = 0x03;
	sendPacket[1] = 0xf3;
	sendPacket[2] = 0xf3;
	sendPacket[3] = 0x0e;
	sendPacket[4] = 0x0e;

	stringstream responseStream;
	responseStream.str("");
	ros::Rate encoderRate = 7;

	bool isFirst = true;

	while(!_ISEND != 6 ) {

		encoderRate.sleep();
//		EnterCriticalSection(&_comunicate);
		pthread_mutex_lock(&__comunicate_mutex);

		_baseConnection->writeBytes( sendPacket, sendPacketSize );
		int readRealSize = _baseConnection->readBytes(receivePacket,encoderPacketSize);
		ROS_DEBUG("Wheel encoder read real size : %d " , readRealSize);
//		printf("read size : %d \t",readRealSize);
//		for(int i=0 ; i<readRealSize ; i++) {
//			printf("%02x ", receivePacket[i]);
//		}
//		printf("\n");

		if(readRealSize == 0) {
			ROS_INFO("Wheel encoder read real size 0");
		} else if(readRealSize < deltaPacketSize) {
//			f1 f1 4f 4b 04
//			char* buff = new char[10];
//			for(int i=0 ; i<readRealSize ; i++) {
//				sprintf(buff, "%02x ", receivePacket[i]);
//				responseStream << buff;
//			}
//			delete buff;
//			responseStream.str("");
			_baseConnection->resetConnection();
			encoderRate.sleep();
		} else if(readRealSize == deltaPacketSize) {
//			01 f1 f1 f1 4f 4b 04
			ROS_DEBUG("Delta move finished...\n");

			bool isTrue = true;
			if (receivePacket[0] != 0x01) {
				isTrue = false;
			} else if (receivePacket[1] != 0xF1) {
				isTrue = false;
			} else if (receivePacket[2] != 0xF1) {
				isTrue = false;
			} else if (receivePacket[3] != 0xF1) {
				isTrue = false;
			} else if (receivePacket[4] != 0x4F) {
				isTrue = false;
			} else if (receivePacket[5] != 0x4b) {
				isTrue = false;
			} else if (receivePacket[6] != 0x04) {
				isTrue = false;
			}
			if(isTrue) {
//				char* buff = new char[10];
//				for(int i=0 ; i<readRealSize ; i++) {
//					sprintf(buff, "%02x ", receivePacket[i]);
//					responseStream << buff;
//				}
//				delete buff;
//				responseStream.str("");
				_isDeltaMove = false;
				_deltaMoveValue = 1;
			}
		} else if(readRealSize < encoderPacketSize) {
//			char* buff = new char[10];
//			for(int i=0 ; i<readRealSize ; i++) {
//				sprintf(buff, "%02x ", receivePacket[i]);
//				responseStream << buff;
//			}
//			delete buff;
//			responseStream.str("");

			bool isTrue = true;
			if(!isTrue) {
				isTrue = true;
				if (receivePacket[readRealSize-7] != 0x01) {
					isTrue = false;
				} else if (receivePacket[readRealSize-6] != 0xF1) {
					isTrue = false;
				} else if (receivePacket[readRealSize-5] != 0xF1) {
					isTrue = false;
				} else if (receivePacket[readRealSize-4] != 0xF1) {
					isTrue = false;
				} else if (receivePacket[readRealSize-3] != 0x4F) {
					isTrue = false;
				} else if (receivePacket[readRealSize-2] != 0x4b) {
					isTrue = false;
				} else if (receivePacket[readRealSize-1] != 0x04) {
					isTrue = false;
				}

				if(isTrue) {
					_isDeltaMove = false;
					_deltaMoveValue = 1;
				}
			}
			if(!isTrue) {
				isTrue = true;
				if (receivePacket[0] != 0x01) {
					isTrue = false;
				} else if (receivePacket[1] != 0xF1) {
					isTrue = false;
				} else if (receivePacket[2] != 0xF1) {
					isTrue = false;
				} else if (receivePacket[3] != 0xF1) {
					isTrue = false;
				} else if (receivePacket[4] != 0x4F) {
					isTrue = false;
				} else if (receivePacket[5] != 0x4b) {
					isTrue = false;
				} else if (receivePacket[6] != 0x04) {
					isTrue = false;
				}

				if(isTrue) {
					_isDeltaMove = false;
					_deltaMoveValue = 1;
				}
			}
			_isDeltaMove = false;
			_deltaMoveValue = 1;
			_baseConnection->resetConnection();
			encoderRate.sleep();
		} else if(readRealSize == encoderPacketSize) {
//			char* buff = new char[10];
//			for(int i=0 ; i<readRealSize ; i++) {
//				sprintf(buff, "%02x ", receivePacket[i]);
//				responseStream << buff;
//			}
//			cout << responseStream.str() << endl;
//			delete buff;
//			LOG4CXX_DEBUG(wheel_logger, "correct encoder Packet : " << readRealSize << " : " << responseStream.str());
//			responseStream.str("");

			bool isTrue = true;


//			read size : 23 	01 f1 f1 f1 4f 4b 04 01 f1 f1 f1 4f 4b 04 01 f1 f1 f4 4f 4b 04 45 4c
//			read size : 23 	30 31 31 36 00 0d 74 05 d5 df 00 00 86 f0 01 f1 f1 f4 4f 4b 04 45 4c

			for(int st = 0; st < 23 - 7; st++) {
				if (receivePacket[st+0] != 0x01) {
					isTrue = false;
				} else if (receivePacket[st+1] != 0xF1) {
					isTrue = false;
				} else if (receivePacket[st+2] != 0xF1) {
					isTrue = false;
				} else if (receivePacket[st+3] != 0xF1) {
					isTrue = false;
				} else if (receivePacket[st+4] != 0x4F) {
					isTrue = false;
				} else if (receivePacket[st+5] != 0x4b) {
					isTrue = false;
				} else if (receivePacket[st+6] != 0x04) {
					isTrue = false;
				}
				if(isTrue) {
					break;
				}
			}
			if(isTrue) {
				//				char* buff = new char[10];
				//				for(int i=0 ; i<readRealSize ; i++) {
				//					sprintf(buff, "%02x ", receivePacket[i]);
				//					responseStream << buff;
				//				}
				//				delete buff;
				//				responseStream.str("");
				_isDeltaMove = false;
				_deltaMoveValue = 1;
				_baseConnection->resetConnection();
				encoderRate.sleep();
			} else {
				indexPoint = 0;
				if(_isDeltaMove) {
					bool isSame = true;
					for(int ind = 0; ind < encoderPacketSize -1; ind++) {
						if(receivePacket[ind] != oldEncoderPacket[ind]) {
							isSame = false;
						}
					}

					if(isSame) {
						_oldEncoderCount++;
					} else {
						_oldEncoderCount = 0;
						for(int ind = 0; ind < encoderPacketSize; ind++) {
							oldEncoderPacket[ind] = receivePacket[ind];
						}
					}
					if(_oldEncoderCount > 10) {
						_isDeltaMove = false;
						_deltaMoveValue = 1;
						_oldEncoderCount = 0;
					}
				}

				if(isFirst) {
					isFirst = false;
					_wheel_encoder_core->parseAndSetWheelEncoderInitDataPacket(encoderPacketSize,receivePacket,indexPoint);
				}
				if(_wheel_encoder_core->isWheelEncoderPacket(receivePacket,indexPoint)) {
					if(RETURN_SUCCESS == _wheel_encoder_core->parseAndSetWheelEncoderDataPacket(encoderPacketSize,receivePacket,indexPoint)) {

					}
				} else {
					ROS_ERROR("\nfail : [_updateBase] broken or checksum error\n");
					_baseConnection->resetConnection();
					encoderRate.sleep();
				}
			}


		}

//		LeaveCriticalSection(&_comunicate);
		pthread_mutex_unlock(&__comunicate_mutex);
	}

	delete[] sendPacket;
	delete[] receivePacket;
	delete[] deltaResponsePacket;
	delete[] oldEncoderPacket;
}


void ERobotCore::_sendBaseResponse() {
	int wheelLoopCount = 0;
	_deltaMoveValue = 1;
	int _oldDeltaMoveValue = 1;

	while(!_ISEND ) {
		pthread_mutex_lock(&__comunicate_mutex);
		wheelLoopCount++;
		if(_oldDeltaMoveValue != _deltaMoveValue) {
			silbot3_msgs::Device_Wheel_Msg successMessage;
			successMessage.command = EROBOT_COMMAND_WHEEL_MOVE_TO_DELTA_XYT;
			successMessage.iParams.push_back(_deltaMoveValue);
			this->_deltaPublisher.publish(successMessage);
			wheelLoopCount = 0;
			_oldDeltaMoveValue = _deltaMoveValue;
		} else if(wheelLoopCount >= 10) {
			silbot3_msgs::Device_Wheel_Msg successMessage;
			successMessage.command = EROBOT_COMMAND_WHEEL_MOVE_TO_DELTA_XYT;
			successMessage.iParams.push_back(_deltaMoveValue);
			this->_deltaPublisher.publish(successMessage);
			wheelLoopCount = 0;
			_oldDeltaMoveValue = _deltaMoveValue;
		}
		pthread_mutex_unlock(&__comunicate_mutex);
		ros::Duration(0.2).sleep();
	}

}

void ERobotCore::_updateBodyPosition() {
	ros::Duration(2).sleep();
	ROS_INFO("ERobotCore::_updateBody start");
	int state			= RETURN_FAILED;
	int indexPoint		= 0;
	int successCount	= 0;
	int loopSize = 2;

	int armSendPacketSize 				= 23;
	int pantiltSendPacketSize			= 14;
	int sendPacketAllSize				= armSendPacketSize+pantiltSendPacketSize; // all


	int sendPacketArmFullSize			= armSendPacketSize+pantiltSendPacketSize+armSendPacketSize;
	int sendPacketPantiltFullSize		= armSendPacketSize+pantiltSendPacketSize+pantiltSendPacketSize;
	int sendPacketAllFullSize			= armSendPacketSize+pantiltSendPacketSize+armSendPacketSize+pantiltSendPacketSize;

	int armReadPacketSize 				= 17;
	int pantiltReadPacketSize			= 11;
	int readPacketSize					= 17+11; // all

	double updatePeriod = 0.1;

	ROS_INFO("UPDATE_BODY_PERIOD : %f", updatePeriod);

	unsigned char* sendAllPacket		= new unsigned char[sendPacketAllSize];
	unsigned char* receivePacket		= new unsigned char[readPacketSize];

	unsigned char* sendAllFullPacket		= new unsigned char[sendPacketAllFullSize];
	unsigned char* sendPantiltFullPacket	= new unsigned char[sendPacketPantiltFullSize];
	unsigned char* sendArmFullPacket		= new unsigned char[sendPacketArmFullSize];

	short armAngles[6]= {0,0,0,0,0,0};
	short armSpeeds[6]= {0,0,0,0,0,0};

	//	clock_t start, end, el, el1, el2, el3;

	clock_t tend, tstart;
	tstart = 0;
	while(!_ISEND && !isTest /* mock mode*/) {
		//		start = clock();
		memset(sendAllPacket, 0x00, sendPacketAllSize);
		unsigned char* currentPacket = sendAllPacket;
		int currentSendIndex = 0;
		if(_isArmUpdate && _isPantiltUpdate) {
			memset(sendAllFullPacket, 0x00, sendPacketAllFullSize);
			_arm_core->getArmCommandPacket(sendAllFullPacket);
			_pantilt_core->addPantiltCommandPacket(sendPantiltFullPacket,armSendPacketSize);

			_isArmUpdate = false;
			_isPantiltUpdate = false;
			currentPacket = sendAllFullPacket;
			currentSendIndex = sendPacketAllSize;
		} else if(_isArmUpdate) {
			memset(sendArmFullPacket, 0x00, sendPacketArmFullSize);
			_arm_core->getArmCommandPacket(sendArmFullPacket);
			_isArmUpdate = false;
			currentPacket = sendArmFullPacket;
			currentSendIndex = armSendPacketSize;


		} else if(_isPantiltUpdate) {
			memset(sendPantiltFullPacket, 0x00, sendPacketPantiltFullSize);
			_pantilt_core->getPantiltCommandPacket(sendPantiltFullPacket);
			_isPantiltUpdate = false;
			currentPacket = sendPantiltFullPacket;
			currentSendIndex = pantiltSendPacketSize;
		} else {

		}
		//		_arm_core->setArmValue(EROBOT_COMMAND_ARM_GET_POSITION, armAngles, armSpeeds);
		//		_pantilt_core->setPantiltValue(EROBOT_COMMAND_PANTILT_XTION_GET_POSITION, 0,0,0,0,0,0);
		_arm_core->addGetPositionPacket(currentPacket, currentSendIndex);
		_pantilt_core->addGetPositionPacket(currentPacket,currentSendIndex+armSendPacketSize);

		//		printf("\n send packet : ");
		//		for(int i=0 ; i<currentSendIndex+sendPacketAllSize ; i++) {
		//			printf("%02x ", currentPacket[i]);
		//		}
		//		printf("\n");
		_bodyConnection->writeBytes( currentPacket, currentSendIndex+sendPacketAllSize );
		int readSize1 = _bodyConnection->readBytes(receivePacket,readPacketSize);
		if(readSize1 < readPacketSize) {

			tend = clock();
			double delay = 0.39;
			if(tstart != 0) {
				delay = updatePeriod - (double)((tstart - tend)/1000);
			}
			ros::Duration(delay).sleep();
			tstart = clock();
			continue;
		}

		indexPoint = 0;
		for(int i=0 ; i<loopSize ; i++) {
			if(_arm_core->isArmPacket(receivePacket,indexPoint)) {
				int armReadPacketSize	= ERobotFunctionFactory::getInstance()->getReadPacketSize(EROBOT_TYPE_ARM);
				if(RETURN_SUCCESS == _arm_core->parseAndSetArmDataPacket(armReadPacketSize, receivePacket, indexPoint)) {
					indexPoint += armReadPacketSize;
					successCount++;
				}
			} else if(_pantilt_core->isPantiltPacket(receivePacket,indexPoint)) {
				if(RETURN_SUCCESS == _pantilt_core->parseAndSetPantiltDataPacket(pantiltReadPacketSize,receivePacket,indexPoint)) {
					indexPoint += pantiltReadPacketSize;
					successCount++;
				}
			} else {
				ROS_ERROR("fail : [_updateBodyPosition] broken or checksum error");
				_bodyConnection->resetConnection();
				ros::Duration(0.1).sleep();
			}
		}

		if(successCount < loopSize) {
			ROS_ERROR("something wrong in [_updateBodyPosition] ");
		}

		tend = clock();
		double delay = 0.39;
		if(tstart != 0) {
			delay = updatePeriod - (double)((tstart - tend)/1000);
		}
		ros::Duration(delay).sleep();
		tstart = clock();

	}
	delete[] sendAllPacket;
	delete[] receivePacket;

}



void ERobotCore::_updateBody() {
	ros::Duration(2).sleep();
	ROS_INFO("ERobotCore::_updateBody start");
	int state			= RETURN_FAILED;
	int indexPoint		= 0;
	int successCount	= 0;
	int loopSize = 2;

	int armSendPacketSize 				= 23;
	int pantiltSendPacketSize			= 14;
	int sendPacketAllSize				= armSendPacketSize+pantiltSendPacketSize; // all


	int sendPacketArmFullSize			= armSendPacketSize;
	int sendPacketPantiltFullSize		= pantiltSendPacketSize;
	int sendPacketAllFullSize			= armSendPacketSize+pantiltSendPacketSize;


	unsigned char* sendAllPacket		= new unsigned char[sendPacketAllSize];

	unsigned char* sendAllFullPacket		= new unsigned char[sendPacketAllFullSize];
	unsigned char* sendPantiltFullPacket	= new unsigned char[sendPacketPantiltFullSize];
	unsigned char* sendArmFullPacket		= new unsigned char[sendPacketArmFullSize];

	short armAngles[6]= {0,0,0,0,0,0};
	short armSpeeds[6]= {0,0,0,0,0,0};

	clock_t tend, tstart;
	tstart = 0;
	int base_count = 0;

	ros::Rate bodyRate = 10;
	while(!_ISEND) {

		if((_rightArmQueue.size() != 0 || _leftArmQueue.size() != 0) && _pantiltQueue.size() != 0) {
			memset(sendAllFullPacket, 0x00, sendPacketAllFullSize);

			if(_rightArmQueue.size() != 0) {
				short* rPop = (short*)_rightArmQueue.pop();
				_arm_core->setRightValue(rPop);
				delete rPop;
			}
			if(_leftArmQueue.size() != 0) {
				short* lPop = (short*)_leftArmQueue.pop();
				_arm_core->setLeftValue(lPop);
				delete lPop;
			}
			_arm_core->getArmCommandPacket(sendAllFullPacket);

			if(_pantiltQueue.size() != 0) {
				unsigned char* pantiltData = (unsigned char*)_pantiltQueue.pop();
				_pantilt_core->setHeadValue(pantiltData);
				delete pantiltData;
			}

			_pantilt_core->addPantiltCommandPacket(sendAllFullPacket,armSendPacketSize);

//			char* buff = new char[10];
//			ecfp << base_count++ << " : "<< tstart <<  " : " << endl;
//				for(int i=0 ; i<sendPacketAllFullSize ; i++) {
//				sprintf(buff, "%02x ", sendAllFullPacket[i]);
//				ecfp << buff;
//			}
//			ecfp << endl;

			_isArmUpdate = false;
			_isPantiltUpdate = false;
			_bodyConnection->writeBytes( sendAllFullPacket, sendPacketAllFullSize );
			// printf("all : ");
			// for(int i=0 ; i<sendPacketAllFullSize ; i++) {
			// 	printf("%02x ", sendAllFullPacket[i]);
			// }
			// printf("\n");
	// cout << "1" << endl;

		} else if(_rightArmQueue.size() != 0 || _leftArmQueue.size() != 0) {
			_isArmUpdate = false;
			memset(sendArmFullPacket, 0x00, sendPacketArmFullSize);
			if(_rightArmQueue.size() != 0) {
				short* rPop = (short*)_rightArmQueue.pop();
				_arm_core->setRightValue(rPop);
				delete rPop;
			}
			if(_leftArmQueue.size() != 0) {
				short* lPop = (short*)_leftArmQueue.pop();
				_arm_core->setLeftValue(lPop);
				delete lPop;
			}
			_arm_core->getArmCommandPacket(sendArmFullPacket);
			_bodyConnection->writeBytes( sendArmFullPacket, sendPacketArmFullSize );
			// printf("arm : ");
			// for(int i=0 ; i<sendPacketArmFullSize ; i++) {
			// 	printf("%02x ", sendArmFullPacket[i]);
			// }
			// printf("\n");
	// cout << "2" << endl;
		} else if(_pantiltQueue.size() != 0) {
			memset(sendPantiltFullPacket, 0x00, sendPacketPantiltFullSize);
			_isPantiltUpdate = false;

			if(_pantiltQueue.size() != 0) {
				unsigned char* pantiltData = (unsigned char*)_pantiltQueue.pop();
				_pantilt_core->setHeadValue(pantiltData);
				delete pantiltData;
			}

			_pantilt_core->addPantiltCommandPacket(sendPantiltFullPacket,0);
			_bodyConnection->writeBytes( sendPantiltFullPacket, sendPacketPantiltFullSize );

			// printf("head : ");
			// for(int i=0 ; i<sendPacketPantiltFullSize ; i++) {
			// 	printf("%02x ", sendPantiltFullPacket[i]);
			// }
			// printf("\n");
	// cout << "3" << endl;
	// cout << sendPantiltFullPacket << endl;

		} else {
			bodyRate.sleep();
		}

		bodyRate.sleep();
	}
	delete[] sendAllPacket;
	delete[] sendAllFullPacket;
	delete[] sendPantiltFullPacket;
	delete[] sendArmFullPacket;

}


// void ERobotCore::_updateBody() {
// 	ros::Duration(2).sleep();
// 	ROS_INFO("ERobotCore::_updateBody start");
// 	int state			= RETURN_FAILED;
// 	int indexPoint		= 0;
// 	int successCount	= 0;
// 	int loopSize = 2;

// 	int armSendPacketSize 				= 23;
// 	int pantiltSendPacketSize			= 14;
// 	int sendPacketAllSize				= armSendPacketSize+pantiltSendPacketSize; // all

// 	short armAngles[6]= {0,0,0,0,0,0};
// 	short armSpeeds[6]= {0,0,0,0,0,0};

// 	stringstream commandStream;
// 	ros::Duration bodyDelay = ros::Duration(0.1);


// 	unsigned char* sendAllPacket		= new unsigned char[sendPacketAllSize];
// 	unsigned char* receivePacket		= new unsigned char[readPacketSize];

// 	unsigned char* sendAllFullPacket		= new unsigned char[sendPacketAllFullSize];
// 	unsigned char* sendPantiltFullPacket	= new unsigned char[sendPacketPantiltFullSize];
// 	unsigned char* sendArmFullPacket		= new unsigned char[sendPacketArmFullSize];


// 	_pantiltQueue.clear();
// 	_rightArmQueue.clear();
// 	_leftArmQueue.clear();

// 	if(depth_sensor_type.compare("kinect") != 0 ){
// 		// Erobot Ver 1.5 Body

// 		//int bodySendPacketSize 				= 32;
// 		//unsigned char* sendBodyPacket		= new unsigned char[bodySendPacketSize];

// 		while(!_ISEND) {
// 			//		LOG4CXX_DEBUG(body_logger, "updateBody time : ");
// 			//		int trigger = _bodyTriggerQueue.pop();
// 			//		LOG4CXX_DEBUG(body_logger, "_bodyTriggerQueue.pop() queue size " << _bodyTriggerQueue.size());

// 			if((_rightArmQueue.size() != 0 || _leftArmQueue.size() != 0) && _pantiltQueue.size() != 0) {
// 				isTeaching = false;
// 				if(_rightArmQueue.size() != 0) {
// 					short* rPop = (short*)_rightArmQueue.pop();
// 					//				LOG4CXX_DEBUG(body_logger, "rPop  rightValue: " << rPop[0]<< ", "<< rPop[1]<< ", "<< rPop[2]<< ", "<< rPop[3]<< ", "<< rPop[4]<< ", "<< rPop[5]);
// 					_body_core->setRightValue(rPop);
// 					delete rPop;
// 				}
// 				if(_leftArmQueue.size() != 0) {
// 					short* lPop = (short*)_leftArmQueue.pop();
// 					//				LOG4CXX_DEBUG(body_logger, "lPop  leftValue: " << lPop[0]<< ", "<< lPop[1]<< ", "<< lPop[2]<< ", "<< lPop[3]<< ", "<< lPop[4]<< ", "<< lPop[5]);
// 					_body_core->setLeftValue(lPop);
// 					delete lPop;
// 				}

// 				if(_pantiltQueue.size() != 0) {
// 					unsigned char* pantiltData = (unsigned char*)_pantiltQueue.pop();
// 					_body_core->setHeadValue(pantiltData);
// 					delete pantiltData;
// 				}

// 				memset(sendBodyPacket, 0x00, bodySendPacketSize);
// 				_body_core->setCommandValue(EROBOT_COMMAND_BODY_MOVE_ALL);
// 				_body_core->getBodyCommandPacket(sendBodyPacket);

// 				char* buff = new char[10];
// 				for(int i=0 ; i<bodySendPacketSize ; i++) {
// 					sprintf(buff, "%02x ", sendBodyPacket[i]);
// 					commandStream << buff;
// 				}
// 				delete[] buff;
// 				if(!isTest) {
// 					_bodyConnection->writeBytes( sendBodyPacket, bodySendPacketSize );
// 				}
// 				ROS_INFO("%s",commandStream.str().c_str());
// 				commandStream.str("");

// 			} else if(_rightArmQueue.size() != 0 || _leftArmQueue.size() != 0) {
// 				isTeaching = false;
// 				if(_rightArmQueue.size() != 0) {
// 					short* rPop = _rightArmQueue.pop();
// 					//				LOG4CXX_DEBUG(body_logger, "rPop  rightValue: " << rPop[0]<< ", "<< rPop[1]<< ", "<< rPop[2]<< ", "<< rPop[3]<< ", "<< rPop[4]<< ", "<< rPop[5]);
// 					_body_core->setRightValue(rPop);
// 					delete rPop;
// 				}
// 				if(_leftArmQueue.size() != 0) {
// 					short* lPop = _leftArmQueue.pop();
// 					//				LOG4CXX_DEBUG(body_logger, "lPop  leftValue: " << lPop[0]<< ", "<< lPop[1]<< ", "<< lPop[2]<< ", "<< lPop[3]<< ", "<< lPop[4]<< ", "<< lPop[5]);
// 					_body_core->setLeftValue(lPop);
// 					delete lPop;
// 				}
// 				_body_core->setCommandValue(EROBOT_COMMAND_BODY_MOVE_ALL);
// 				_body_core->setHeadValueZero();
// 				_body_core->getBodyCommandPacket(sendBodyPacket);

// 				char* buff = new char[10];
// 				for(int i=0 ; i<bodySendPacketSize ; i++) {
// 					sprintf(buff, "%02x ", sendBodyPacket[i]);
// 					commandStream << buff;
// 				}
// 				delete[] buff;
// 				if(!isTest) {
// 					_bodyConnection->writeBytes( sendBodyPacket, bodySendPacketSize );
// 				}
// 				//						cout << commandStream.str() << endl;
// 				commandStream.str("");
// 			} else if(_pantiltQueue.size() != 0) {
// 				isTeaching = false;
// 				memset(sendBodyPacket, 0x00, bodySendPacketSize);
// 				unsigned char* pantiltData = (unsigned char*)_pantiltQueue.pop();

// 				char* buff2 = new char[10];
// 				for(int i=0 ; i<14 ; i++) {
// 					sprintf(buff2, "%02x ", pantiltData[i]);
// 					commandStream << buff2;
// 				}
// 				delete[] buff2;
// 				commandStream.str("");

// 				_body_core->setHeadValue(pantiltData);
// 				_body_core->setCommandValue(EROBOT_COMMAND_BODY_MOVE_ALL);
// 				_body_core->getBodyCommandPacket(sendBodyPacket);
// 				char* buff = new char[10];
// 				for(int i=0 ; i<bodySendPacketSize ; i++) {
// 					sprintf(buff, "%02x ", sendBodyPacket[i]);
// 					commandStream << buff;
// 				}
// 				delete[] buff;
// 				if(!isTest) {
// 					_bodyConnection->writeBytes( sendBodyPacket, bodySendPacketSize );
// 				}
// 				//				cout << commandStream.str() << endl;
// 				commandStream.str("");
// 				delete pantiltData;
// 			}
// 			bodyDelay.sleep();
// 		}
// 		delete[] sendBodyPacket;
// 	}else {
// 		// Erobot Ver 1.5 Body

// 		int bodySendPacketSize 				= 29;
// 		unsigned char* sendBodyPacket		= new unsigned char[bodySendPacketSize];

// 		while(!_ISEND) {
// 			//		LOG4CXX_DEBUG(body_logger, "updateBody time : ");
// 			//		int trigger = _bodyTriggerQueue.pop();
// 			//		LOG4CXX_DEBUG(body_logger, "_bodyTriggerQueue.pop() queue size " << _bodyTriggerQueue.size());



// 			if((_rightArmQueue.size() != 0 || _leftArmQueue.size() != 0) && _pantiltQueue.size() != 0) {
// 				isTeaching = false;
// 				if(_rightArmQueue.size() != 0) {
// 					short* rPop = (short*)_rightArmQueue.pop();
// 					//				LOG4CXX_DEBUG(body_logger, "rPop  rightValue: " << rPop[0]<< ", "<< rPop[1]<< ", "<< rPop[2]<< ", "<< rPop[3]<< ", "<< rPop[4]<< ", "<< rPop[5]);
// 					_body_core->setRightValue(rPop);
// 					delete rPop;
// 				}
// 				if(_leftArmQueue.size() != 0) {
// 					short* lPop = (short*)_leftArmQueue.pop();
// 					//				LOG4CXX_DEBUG(body_logger, "lPop  leftValue: " << lPop[0]<< ", "<< lPop[1]<< ", "<< lPop[2]<< ", "<< lPop[3]<< ", "<< lPop[4]<< ", "<< lPop[5]);
// 					_body_core->setLeftValue(lPop);
// 					delete lPop;
// 				}

// 				if(_pantiltQueue.size() != 0) {
// 					unsigned char* pantiltData = (unsigned char*)_pantiltQueue.pop();
// 					_body_core->setHeadValue(pantiltData);
// 					delete pantiltData;
// 				}

// 				memset(sendBodyPacket, 0x00, bodySendPacketSize);
// 				_body_core->setCommandValue(EROBOT_COMMAND_BODY_MOVE_ALL);
// 				_body_core->getBodyCommandPacket(sendBodyPacket);

// 				char* buff = new char[10];
// 				for(int i=0 ; i<bodySendPacketSize ; i++) {
// 					sprintf(buff, "%02x ", sendBodyPacket[i]);
// 					commandStream << buff;
// 				}
// 				delete[] buff;
// 				if(!isTest) {
// 					_bodyConnection->writeBytes( sendBodyPacket, bodySendPacketSize );
// 				}
// 				//cout << "all : " << commandStream.str() << endl;
// 				commandStream.str("");

// 			} else if(_rightArmQueue.size() != 0 || _leftArmQueue.size() != 0) {
// 				isTeaching = false;
// 				if(_rightArmQueue.size() != 0) {
// 					short* rPop = _rightArmQueue.pop();
// 					//				LOG4CXX_DEBUG(body_logger, "rPop  rightValue: " << rPop[0]<< ", "<< rPop[1]<< ", "<< rPop[2]<< ", "<< rPop[3]<< ", "<< rPop[4]<< ", "<< rPop[5]);
// 					_body_core->setRightValue(rPop);
// 					delete rPop;
// 				}
// 				if(_leftArmQueue.size() != 0) {
// 					short* lPop = _leftArmQueue.pop();
// 					//				LOG4CXX_DEBUG(body_logger, "lPop  leftValue: " << lPop[0]<< ", "<< lPop[1]<< ", "<< lPop[2]<< ", "<< lPop[3]<< ", "<< lPop[4]<< ", "<< lPop[5]);
// 					_body_core->setLeftValue(lPop);
// 					delete lPop;
// 				}
// 				_body_core->setCommandValue(EROBOT_COMMAND_BODY_MOVE_ALL);
// 				_body_core->setHeadValueZero();
// 				_body_core->getBodyCommandPacket(sendBodyPacket);

// 				char* buff = new char[10];
// 				for(int i=0 ; i<bodySendPacketSize ; i++) {
// 					sprintf(buff, "%02x ", sendBodyPacket[i]);
// 					commandStream << buff;
// 				}
// 				delete[] buff;
// 				if(!isTest) {
// 					_bodyConnection->writeBytes( sendBodyPacket, bodySendPacketSize );
// 				}
// 				//cout <<  "arm : " <<commandStream.str() << endl;
// 				commandStream.str("");
// 			} else if(_pantiltQueue.size() != 0) {
// 				isTeaching = false;
// 				memset(sendBodyPacket, 0x00, bodySendPacketSize);
// 				unsigned char* pantiltData = (unsigned char*)_pantiltQueue.pop();

// 				char* buff2 = new char[10];
// 				for(int i=0 ; i<14 ; i++) {
// 					sprintf(buff2, "%02x ", pantiltData[i]);
// 					commandStream << buff2;
// 				}
// 				delete[] buff2;
// 				commandStream.str("");

// 				_body_core->setHeadValue(pantiltData);
// 				_body_core->setCommandValue(EROBOT_COMMAND_BODY_MOVE_ALL);
// 				_body_core->getBodyCommandPacket(sendBodyPacket);
// 				char* buff = new char[10];
// 				for(int i=0 ; i<bodySendPacketSize ; i++) {
// 					sprintf(buff, "%02x ", sendBodyPacket[i]);
// 					commandStream << buff;
// 				}
// 				delete[] buff;
// 				if(!isTest) {
// 					_bodyConnection->writeBytes( sendBodyPacket, bodySendPacketSize );
// 				}
// 				//cout <<  "head : " <<commandStream.str() << endl;
// 				commandStream.str("");
// 				delete pantiltData;
// 			}
// 			bodyDelay.sleep();
// 		}
// 		delete[] sendBodyPacket;
// 	}
// }

void ERobotCore::_updateSensor() {
	ros::Duration(2).sleep();
	ROS_INFO("ERobotCore::_updateSensor start");
	int state			= RETURN_FAILED;
	int indexPoint		= 0;
	int successCount	= 0;
	int readSize = 0;

	int readPacketSize				= 13;  // ultra
	int sendPacketSize				= 5;

	unsigned char* sendPacket		= new unsigned char[sendPacketSize];
	unsigned char* receivePacket	= new unsigned char[readPacketSize];
	while(!_ISEND && !isTest /* mock mode*/) {
		memset(sendPacket, 0x00, sendPacketSize);
		indexPoint = 0;
		_ultrasonic_core->addUltrsonicCommandPacket( EROBOT_COMMAND_ULTRASONIC_GET_DATA, sendPacket, indexPoint);
		_sensorConnection->writeBytes( sendPacket, sendPacketSize );

		readSize = _sensorConnection->readBytes(receivePacket,readPacketSize);

		//				printf("ultra : %d : ",readSize);
		//				char* buff = new char[10];
		//				for(int i=0 ; i<readSize ; i++) {
		//					printf("%02x ", receivePacket[i]);
		//				}
		//				delete[] buff;
		//				cout << endl;

		if( readSize < readPacketSize) {
			ros::Duration(0.7).sleep();
			continue;
		}
		if(_ultrasonic_core->isUltrasonicPacket(receivePacket,indexPoint)) {
			int ultraReadPacketSize	= ERobotFunctionFactory::getInstance()->getReadPacketSize(EROBOT_TYPE_ULTRASONIC);
			if(RETURN_SUCCESS == _ultrasonic_core->parseAndSetUltrsonicDataPacket(ultraReadPacketSize, receivePacket, indexPoint)) {
				indexPoint += ultraReadPacketSize;
				successCount++;
			}
		} else {
			cout << "fail : [_updateSensor] broken or checksum error" << endl;
			_sensorConnection->resetConnection();
			ros::Duration(1).sleep();
		}

		ros::Duration(0.25).sleep();
	}
	delete[] sendPacket;
	delete[] receivePacket;
}
void ERobotCore::_updateEtc() {

	ros::Duration(2).sleep();
	ROS_INFO("ERobotCore::_updateEtc start");
	int state			= RETURN_FAILED;
	int indexPoint		= 0;
	int successCount	= 0;
	int readSize = 0;
	int loopSize = 1;

	int ledSize = 15;
	unsigned char* ledPacket		= new unsigned char[ledSize];

	_led_core->setLEDValue(0xF2, 0, 0, 0, 0);
	_led_core->addLEDCommandPacket( ledPacket, 0);
	_etcConnection->writeBytes( ledPacket, ledSize );

	ros::Duration(1.5).sleep();

	_led_core->setLEDValue(0xF5, 0, 0, 0, 0);
	_led_core->addLEDCommandPacket( ledPacket, 0);
	_etcConnection->writeBytes( ledPacket, ledSize );

	int readPacketSize				= 10;  // touch + battery
	int sendPacketSize				= 5;  // touch + + battery
	int sendAllPacketSize			= 15+5; // touch + led + battery

	unsigned char* sendPacket		= new unsigned char[sendPacketSize];
	unsigned char* sendAllPacket	= new unsigned char[sendAllPacketSize];
	unsigned char* receivePacket	= new unsigned char[readPacketSize];
	while(!_ISEND && !isTest /* mock mode*/) {
		if(!isTouchErrorChecking) {
			pthread_mutex_lock( &__sensor_mutex );
			memset(receivePacket, 0x00, readPacketSize);

			if(_isLedUpdate) {
				memset(sendAllPacket, 0x00, sendAllPacketSize);
				indexPoint = 0;
				successCount = 0;
				_touch_core->addTouchButtonCommandPacket( EROBOT_COMMAND_TOUCHBUTTON_GET_DATA, sendAllPacket, 0);
				_led_core->addLEDCommandPacket( sendAllPacket, 5);
				//				_battery_core->addBatteryCommandPacket(EROBOT_COMMAND_BATTERY_GET_DATA,sendAllPacket,20);


				_etcConnection->writeBytes( sendAllPacket, sendAllPacketSize );
				_isLedUpdate = false;
				//				printf("\n send packet : ");
				//				for(int i=0 ; i<sendAllPacketSize ; i++) {
				//					printf("%02x ", sendAllPacket[i]);
				//				}
				//				printf("\n");
			} else {
				memset(sendPacket, 0x00, sendPacketSize);
				indexPoint = 0;
				successCount = 0;
				_touch_core->addTouchButtonCommandPacket( EROBOT_COMMAND_TOUCHBUTTON_GET_DATA, sendPacket, 0);
				//				_battery_core->addBatteryCommandPacket(EROBOT_COMMAND_BATTERY_GET_DATA,sendPacket,5);


				_etcConnection->writeBytes( sendPacket, sendPacketSize );
			}


			readSize = _etcConnection->readBytes(receivePacket,readPacketSize);

			//			printf("\n receive packet : ");
			//			for(int i=0 ; i<readSize ; i++) {
			//				printf("%02x ", receivePacket[i]);
			//			}
			//			printf("\n");

			if( readSize < readPacketSize) {
				_etcConnection->resetConnection();
				pthread_mutex_unlock( &__sensor_mutex );
				ros::Duration(0.2).sleep();
				continue;
			}


			for(int i=0 ; i<loopSize ; i++) {
				if(_touch_core->isTouchButtonPacket(receivePacket,indexPoint)) {
					int touchReadPacketSize	= ERobotFunctionFactory::getInstance()->getReadPacketSize(EROBOT_TYPE_TOUCHBUTTON);
					if(RETURN_SUCCESS == _touch_core->parseAndSetTouchButtonDataPacket(touchReadPacketSize, receivePacket, indexPoint)) {
						indexPoint += touchReadPacketSize;
						successCount++;
					}
				}
				//				else if(_battery_core->isBatteryPacket(receivePacket,indexPoint)) {
				//					int batteryReadPacketSize = 8;
				//					if(RETURN_SUCCESS == _battery_core->parseAndSetBatteryDataPacket(batteryReadPacketSize,receivePacket,indexPoint)) {
				//						indexPoint += batteryReadPacketSize;
				//						successCount++;
				//					}
				//				}
				else {
					//				cout << "fail : [_updateEtc] broken or checksum error" << endl;
					_etcConnection->resetConnection();
					ros::Duration(0.2).sleep();
				}
			}

			if(successCount < loopSize) {
				cout << "something wrong in [_updateEtc] " << endl;
			}
			pthread_mutex_unlock( &__sensor_mutex );
		}
		ros::Duration(0.15).sleep();
	}
	delete[] sendPacket;
	delete[] receivePacket;

}

void ERobotCore::_updateUltrasonic() {

	pthread_mutex_lock( &__sensor_mutex );
	unsigned char* sendPacket = new unsigned char[5];
	unsigned char* receivePacket = new unsigned char[13];
	sendPacket[0] = 0x05;
	sendPacket[1] = 0xF5;
	sendPacket[2] = 0xF5;
	sendPacket[3] = 0xF5;
	sendPacket[4] = 0x0A;

	_baseConnection->writeBytes( sendPacket, 5 );
	ros::Duration(0.05).sleep();
	_baseConnection->readBytes(receivePacket,13);

	pthread_mutex_unlock( &__sensor_mutex );

	delete[] sendPacket;
	delete[] receivePacket;
}

void ERobotCore::_updateTouchButton() {
	int state = RETURN_FAILED;

	int readDataSize = ERobotFunctionFactory::getInstance()->getReadPacketSize(EROBOT_TYPE_TOUCHBUTTON);
	unsigned char* sendPacket = new unsigned char[EROBOT_COMMON_HEADER_PACKET_SIZE + EROBOT_CHECKSUM_PACKET_SIZE];
	unsigned char* receivePacket = new unsigned char[readDataSize];

	_touch_core->getTouchButtonCommandPacket(EROBOT_COMMAND_TOUCHBUTTON_GET_DATA, sendPacket);
	_etcConnection->writeBytes( sendPacket, 5 );

	int readSize = _etcConnection->readBytes(receivePacket,readDataSize);
	if( readSize < readDataSize) {
		return;
	}

	if(_touch_core->isTouchButtonPacket(receivePacket,0)) {
		int touchReadPacketSize	= ERobotFunctionFactory::getInstance()->getReadPacketSize(EROBOT_TYPE_TOUCHBUTTON);
		if(RETURN_SUCCESS == _touch_core->parseAndSetTouchButtonDataPacket(touchReadPacketSize, receivePacket)) {

		}
	}
	delete[] sendPacket;
	delete[] receivePacket;
}

void ERobotCore::_updateLED() {

	int sendPacketSize				= 15;
	unsigned char* sendPacket		= new unsigned char[sendPacketSize];

	memset(sendPacket, 0x00, sendPacketSize);

	_led_core->getLEDCommandPacket( sendPacket);
	_etcConnection->writeBytes( sendPacket, sendPacketSize );

	//	ROS_DEBUG("\n _updateLED send packet : ");
	//	for(int i=0 ; i<sendPacketSize ; i++) {
	//		ROS_DEBUG("%02x ", sendPacket[i]);
	//	}
	//	ROS_DEBUG("\n");


	delete[] sendPacket;
}

void ERobotCore::_updateWheel() {
}


void ERobotCore::_updateArm() {
	ros::Duration(2).sleep();
	_isArmUpdate = false;

	double updateArmPeriod = 0.1;

	ROS_INFO("ERobotCore::_updateArm start period %f" , updateArmPeriod);
	int sendPacketSize				= 23;
	unsigned char* sendPacket		= new unsigned char[sendPacketSize];
	while(!_ISEND && !isTest /* mock mode*/) {
		if(_isArmUpdate) {
			memset(sendPacket, 0x00, sendPacketSize);
			_arm_core->getArmCommandPacket(sendPacket);
			_bodyConnection->writeBytes( sendPacket, sendPacketSize );
			_isArmUpdate = false;
		}
		ros::Duration(updateArmPeriod).sleep();
	}

	delete[] sendPacket;
}

void ERobotCore::_updatePantilt() {
	int state = RETURN_FAILED;
	int sendPacketSize				= 14; // pantilt + arm
	unsigned char* sendPacket		= new unsigned char[sendPacketSize];

	memset(sendPacket, 0x00, sendPacketSize);
	_pantilt_core->getPantiltCommandPacket(sendPacket);
	_bodyConnection->writeBytes( sendPacket, sendPacketSize );

	delete[] sendPacket;
}


void ERobotCore::getWheelEncoderValue(long* data, int* x, int* y, int* t ) {
	//pthread_mutex_lock( &__comunicate_mutex );
	//ROS_DEBUG("-------> get wheel encoder value!");
	_wheel_encoder_core->getWheelEncoderValue(data,x,y,t);
	//pthread_mutex_unlock( &__comunicate_mutex );
}

void ERobotCore::getBatteryValue(int* voltage) {
	pthread_mutex_lock( &__sensor_mutex );
	// _wheel_encoder_core->getBatteryValue(voltage);
	pthread_mutex_unlock( &__sensor_mutex );
}

void ERobotCore::getUltrasonicValue(unsigned short *data, int nDataSize) {
	//EnterCriticalSection(&_sensor_comunicate);
	_ultrasonic_core->getUltrasonicValue(data,nDataSize);
	//LeaveCriticalSection(&_sensor_comunicate);
}

int ERobotCore::setWheelValue(unsigned char command, int param1, int param2, int param3, int param4) {
	//EnterCriticalSection(&_comunicate);
	pthread_mutex_lock(&__comunicate_mutex);
	_isWheelUpdate = true;
	_oldEncoderCount = 0;

	if(command == EROBOT_COMMAND_WHEEL_EMERGENCY_STOP) {
		//		printf("EROBOT_COMMAND_WHEEL_EMERGENCY_STOP\n");
		if(_isDeltaMove) {
			_isDeltaMove = false;
			_deltaMoveValue = -1;
		} else {
			_deltaMoveValue = 1;
		}
		int stopPacketSize = 5;
		unsigned char* stopPacket = new unsigned char[stopPacketSize];
		_wheel_core->addWheelStopCommandPacket(stopPacket,0);
		_baseConnection->writeBytes(stopPacket,stopPacketSize);

		ros::Duration(0.1).sleep();
		_baseConnection->writeBytes(stopPacket,stopPacketSize);
		_isWheelUpdate = false;
	} else {
		_wheel_core->setWheelValue(command,param1,param2,param3,param4);
	}

	pthread_mutex_unlock(&__comunicate_mutex);
	return 0;
}

int ERobotCore::setPanTiltValue(unsigned char id, short anglePan, short angleTilt, short speedPan, short speedTilt) {
	pthread_mutex_lock( &__body_mutex );
	//cout << "   ERobotCore::setPanTiltValue " << endl;

	_isPantiltUpdate = true;
	if(id == EROBOT_COMMAND_PANTILT_STOP) {
		_pantiltQueue.clear();
	} else {
		_pantilt_core->setPantiltValue(id, anglePan, angleTilt, 0, speedPan, speedTilt, 0);

		bodyPosition[0] = anglePan;
		bodyPosition[1] = angleTilt;

		unsigned char* pantiltPacket = new unsigned char[14];
		_pantilt_core->getPantiltCommandPacket(pantiltPacket);
		_pantiltQueue.push(pantiltPacket);
	}

	pthread_mutex_unlock( &__body_mutex );
	return 0;
}

int ERobotCore::setXtionPanTiltValue(unsigned char id, short anglePan, short angleTilt, short angleXtion, short speedPan, short speedTilt, short speedXtion) {
	pthread_mutex_lock( &__body_mutex );
	_isPantiltUpdate = true;
	_pantilt_core->setPantiltValue(id, anglePan,angleTilt,angleXtion, speedPan,speedTilt,speedXtion);

	bodyPosition[0] = anglePan;
	bodyPosition[1] = angleTilt;
	bodyPosition[8] = angleXtion;

	unsigned char* pantiltPacket = new unsigned char[14];
	_pantilt_core->getPantiltCommandPacket(pantiltPacket);
	_pantiltQueue.push(pantiltPacket);
	//	_updatePantilt();
	pthread_mutex_unlock( &__body_mutex );
	return 0;
}


int ERobotCore::setLEDValue(unsigned char command, unsigned char red, unsigned char green, unsigned char blue, unsigned char blink) {
	pthread_mutex_lock( &__sensor_mutex );
	_isLedUpdate = true;
	//	printf("ERobotCore::setLEDValue %d %d %d %d ", command, red, green, blue);
	ROS_DEBUG("ERobotCore::setLEDValue %d %d %d %d ", command, red, green, blue);
	_led_core->setLEDValue(command,red,green,blue,blink);
	//_updateLED();
	pthread_mutex_unlock( &__sensor_mutex );
	return 0;
}

void ERobotCore::getTouchButtonValue(unsigned short *data, int nDataSize) {
	pthread_mutex_lock( &__sensor_mutex );
	//	_updateTouchButton();
	_touch_core->getTouchButtonValue(data,nDataSize);
	pthread_mutex_unlock( &__sensor_mutex );
}

int ERobotCore::setArmValue(unsigned char command, short *angles, short *speeds) {

	// cout << "setArmValue" << endl;
	pthread_mutex_lock( &__body_mutex );
	// for (int i = 0 ; i < 6 ; i++ ){
	// 	printf("%d]] %d %d |", i, angles[i], speeds[i]);
	// }
	// printf("\n");
	_isArmUpdate = true;
	int trigger = 0;
	if(command == EROBOT_COMMAND_ARM_STOP) {
		_rightArmQueue.clear();
		_leftArmQueue.clear();

	} else if(command == EROBOT_COMMAND_ARM_MOVE_RIGHT_ARM) {

		bodyPosition[2] = angles[0];
		bodyPosition[3] = angles[1];
		bodyPosition[4] = angles[2];

		short* rightValue = new short[6];
		rightValue[0] = angles[0];
		rightValue[1] = angles[1];
		rightValue[2] = angles[2];
		rightValue[3] = speeds[0];
		rightValue[4] = speeds[1];
		rightValue[5] = speeds[2];
		_rightArmQueue.push(rightValue);


	} else if(command == EROBOT_COMMAND_ARM_MOVE_LEFT_ARM) {

		bodyPosition[5] = angles[0];
		bodyPosition[6] = angles[1];
		bodyPosition[7] = angles[2];

		short* leftValue = new short[6];
		leftValue[0] = angles[3];
		leftValue[1] = angles[4];
		leftValue[2] = angles[5];
		leftValue[3] = speeds[3];
		leftValue[4] = speeds[4];
		leftValue[5] = speeds[5];

		_leftArmQueue.push(leftValue);

	} else {
		bodyPosition[2] = angles[0];
		bodyPosition[3] = angles[1];
		bodyPosition[4] = angles[2];
		bodyPosition[5] = angles[3];
		bodyPosition[6] = angles[4];
		bodyPosition[7] = angles[5];

		short* rightValue = new short[6];
		rightValue[0] = angles[0];
		rightValue[1] = angles[1];
		rightValue[2] = angles[2];
		rightValue[3] = speeds[0];
		rightValue[4] = speeds[1];
		rightValue[5] = speeds[2];

		_rightArmQueue.push(rightValue);
		short* leftValue = new short[6];
		leftValue[0] = angles[3];
		leftValue[1] = angles[4];
		leftValue[2] = angles[5];
		leftValue[3] = speeds[3];
		leftValue[4] = speeds[4];
		leftValue[5] = speeds[5];
		_leftArmQueue.push(leftValue);
	}

	pthread_mutex_unlock( &__body_mutex );
	return 0;
}

int ERobotCore::setArmHomePosition() {
	cout << "ERobotCore::setArmHomePosition" << endl;

	unsigned char* setArmHomePacket		= new unsigned char[5];
	setArmHomePacket[0] = 0x08;
	setArmHomePacket[1] = 0xF8;
	setArmHomePacket[2] = 0xF8;
	setArmHomePacket[3] = 0x07;
	setArmHomePacket[4] = 0x07;

	_bodyConnection->writeBytes( setArmHomePacket, 5 );

	delete[] setArmHomePacket;

}

void ERobotCore::getArmPosition(short* positions) {
	pthread_mutex_lock( &__body_mutex );
	_arm_core->getArmPositons(positions);

	//	int sendPacketSize = 23;
	//	unsigned char* sendPacket = new unsigned char[sendPacketSize];
	//
	//	memset(sendPacket, 0x00, sendPacketSize);
	//	short angles[6] = {0,0,0,0,0,0};
	//	short speeds[6] = {0,0,0,0,0,0};
	//	_arm_core->setArmValue(EROBOT_COMMAND_ARM_GET_POSITION,angles,speeds);
	//	_arm_core->addArmCommandPacket(sendPacket, 0);
	//	_bodyConnection->writeBytes(sendPacket,sendPacketSize);
	//
	//	int receivePacketSize = 17;
	//	unsigned char* receivePacket = new unsigned char[receivePacketSize];
	//	memset(receivePacket, 0x00, receivePacketSize);
	//
	//	int real = _bodyConnection->readBytes(receivePacket,receivePacketSize);
	//	if( real < receivePacketSize) {
	//		ROS_ERROR("ARM_GET_POSITION readBytes size is different need 16 real %d", real);
	//		positions[0] = -999;
	//		pthread_mutex_unlock( &__body_mutex );
	//		return;
	//	}
	//
	//
	//	if(_arm_core->isArmPositionPacket(receivePacket)) {
	//		if(RETURN_SUCCESS == _arm_core->parseAndSetArmPositionDataPacket(receivePacketSize,receivePacket, positions)) {
	//
	//		} else {
	//			positions[0] = -999;
	//		}
	//	} else {
	//		ROS_ERROR("ARM_GET_POSITION receive 17 byte but it is not arm position packet......");
	//		ROS_ERROR("   - expected header packet 01 F8 F8 F8 ");
	//		ROS_ERROR("   - received header packet %02x %02x %02x %02x ",positions[0], positions[1], positions[2], positions[3]);
	//		ROS_ERROR("set default value -999");
	//		positions[0] = -999;
	//		positions[1] = -999;
	//		positions[2] = -999;
	//		positions[3] = -999;
	//		positions[4] = -999;
	//		positions[5] = -999;
	//	}

	pthread_mutex_unlock( &__body_mutex );
}

void ERobotCore::getPantiltPosition(double* positions) {
	pthread_mutex_lock( &__body_mutex );
	_pantilt_core->getPantiltPositons(positions);

	cout << "hi!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" <<endl;

	//	int sendPacketSize = 14;
	//	unsigned char* sendPacket = new unsigned char[sendPacketSize];
	//
	//	memset(sendPacket, 0x00, sendPacketSize);
	//	_pantilt_core->setPantiltValue(EROBOT_COMMAND_PANTILT_GET_POSITION,0,0,0,0,0,0);
	//	_pantilt_core->addPantiltCommandPacket(sendPacket, 0);
	//	_bodyConnection->writeBytes(sendPacket,sendPacketSize);
	//
	//	int receivePacketSize = 11;
	//	unsigned char* receivePacket = new unsigned char[receivePacketSize];
	//	memset(receivePacket, 0x00, receivePacketSize);
	//
	//
	//	int real = _bodyConnection->readBytes(receivePacket,receivePacketSize);
	//
	//	if(real < receivePacketSize) {
	//		ROS_ERROR("PANTILT_DETECTING_ERROR readBytes size is different");
	//		positions[0] = -999;
	//		pthread_mutex_unlock( &__body_mutex );
	//		return;
	//	}
	//
	//	if(_pantilt_core->isPantiltPositionPacket(receivePacket)) {
	//		if(RETURN_SUCCESS == _pantilt_core->parseAndSetPantiltPositionDataPacket(receivePacketSize,receivePacket, positions)) {
	//
	//		} else {
	//			positions[0] = -999;
	//		}
	//	} else {
	//		positions[0] = -999;
	//	}

	pthread_mutex_unlock( &__body_mutex );
}

void ERobotCore::selfDiagnosisWheel(short errorCode[9]) {
}

void ERobotCore::selfDiagnosisWheelNoMove(short errorCode[9]) {

}

void ERobotCore::errorDetectingPantilt(short errorCode[3]) {
	pthread_mutex_lock( &__body_mutex );

	int sendPacketSize = 14;
	unsigned char* sendPacket = new unsigned char[sendPacketSize];

	memset(sendPacket, 0x00, sendPacketSize);
	_pantilt_core->setPantiltValue(EROBOT_COMMAND_ARM_DETECTING_ERROR,0,0,0,0,0,0);
	_pantilt_core->addPantiltCommandPacket(sendPacket, 0);
	_bodyConnection->writeBytes(sendPacket,sendPacketSize);

	int receivePacketSize = 8;
	unsigned char* receivePacket = new unsigned char[receivePacketSize];
	memset(receivePacket, 0x00, receivePacketSize);

	int real = _bodyConnection->readBytes(receivePacket,receivePacketSize);

	if(real < receivePacketSize) {
		ROS_ERROR("PANTILT_DETECTING_ERROR readBytes size is different need is %d , real is %d", receivePacketSize, real);
		errorCode[0] = -1;
		pthread_mutex_unlock( &__body_mutex );
		return;
	}

	if(_pantilt_core->isPantiltErrorPacket(receivePacket)) {
		if(RETURN_SUCCESS == _pantilt_core->parseAndSetPantiltErrorDataPacket(receivePacketSize,receivePacket, errorCode)) {

		} else {
			errorCode[0] = -1;
		}
	} else {
		errorCode[0] = -1;
	}
	pthread_mutex_unlock( &__body_mutex );
}

void ERobotCore::errorDetectingArm(short errorCode[6]) {

	pthread_mutex_lock( &__body_mutex );

	int sendPacketSize = 23;
	unsigned char* sendPacket = new unsigned char[sendPacketSize];

	memset(sendPacket, 0x00, sendPacketSize);
	short angles[6] = {0,0,0,0,0,0};
	short speeds[6] = {0,0,0,0,0,0};
	_arm_core->setArmValue(EROBOT_COMMAND_PANTILT_DETECTING_ERROR,angles,speeds);
	_arm_core->addArmCommandPacket(sendPacket, 0);
	_bodyConnection->writeBytes(sendPacket,sendPacketSize);

	int receivePacketSize = 11;
	unsigned char* receivePacket = new unsigned char[receivePacketSize];
	memset(receivePacket, 0x00, receivePacketSize);

	int real = _bodyConnection->readBytes(receivePacket,receivePacketSize);
	if(real < receivePacketSize) {
		ROS_ERROR("ARM_DETECTING_ERROR readBytes size is different");
		errorCode[0] = -1;
		pthread_mutex_unlock( &__body_mutex );
		return;
	}


	if(_arm_core->isArmErrorPacket(receivePacket)) {
		if(RETURN_SUCCESS == _arm_core->parseAndSetArmErrorDataPacket(receivePacketSize,receivePacket, errorCode)) {

		} else {
			errorCode[0] = -1;
		}
	} else {
		errorCode[0] = -1;
	}
	pthread_mutex_unlock( &__body_mutex );
}


int ERobotCore::errorDetectingTouch(int touchID) {
	isTouchErrorChecking = true;

	pthread_mutex_lock( &__sensor_mutex );

	ros::Duration(0.15).sleep();
	int readPacketSize2				= 10+8;  // touch + battery

	unsigned char* receivePacket2	= new unsigned char[readPacketSize2];
	memset(receivePacket2, 0x00, readPacketSize2);
	_etcConnection->readBytes(receivePacket2,readPacketSize2);

	int result = -1;
	int sendPacketSize = 6;
	unsigned char* sendPacket = new unsigned char[sendPacketSize];

	memset(sendPacket, 0x00, sendPacketSize);

	_touch_core->addTouchButtonErrorPacket(EROBOT_COMMAND_TOUCHBUTTON_DETECTING_ERROR,sendPacket,touchID, 0);


	_etcConnection->writeBytes(sendPacket,sendPacketSize);

	int receivePacketSize = 7;
	unsigned char* receivePacket = new unsigned char[receivePacketSize];
	memset(receivePacket, 0x00, receivePacketSize);

	int real = _etcConnection->readBytes(receivePacket,receivePacketSize);

	if(real < receivePacketSize) {
		ROS_ERROR("TOUCH_DETECTING_ERROR readBytes size is different need is 7, reals is %d", real);
		pthread_mutex_unlock( &__sensor_mutex );
		return -1;
	}

	if(_touch_core->isTouchErrorPacket(receivePacket)) {
		result = _touch_core->parseAndSetTouchErrorDataPacket(receivePacketSize,receivePacket);
	} else {

	}
	pthread_mutex_unlock( &__sensor_mutex );
	isTouchErrorChecking = false;
	return result;
}

void ERobotCore::waitForWheelUpdate() {
	pthread_mutex_lock(&__comunicate_mutex);

	// We wait for helper to change (which is the true indication we are
	// ready) and use a condition variable so we can do this efficiently.
	while (helper == 0)
	{
		pthread_cond_wait(&__cv, &__comunicate_mutex);
	}

	pthread_mutex_unlock(&__comunicate_mutex);
	ros::Duration(1).sleep();
}


void ERobotCore::releaseTorue() {
	cout << "core :: releaseTorue" << endl;

	if(depth_sensor_type.compare("kinect") != 0) { // xtion
		unsigned char packet[32] = {0x08, 0xF8, 0xF8, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
		_bodyConnection->writeBytes( packet, 32 );
	} else {	// kinect
		unsigned char packet[29] = {0x08, 0xF8, 0xF8, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
		_bodyConnection->writeBytes( packet, 29 );
	}

}

void ERobotCore::startArmsTeaching() {
	cout << "core :: startArmsTeaching" << endl;

	_bodyConnection->resetConnection();
		ros::Duration(1).sleep();

	if(depth_sensor_type.compare("kinect") != 0) { // xtion
		unsigned char packet[32] = {0x08, 0xF8, 0xF8, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
		_bodyConnection->writeBytes( packet, 32 );
	} else {	// kinect
		unsigned char packet[29] = {0x08, 0xF8, 0xF8, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
		_bodyConnection->writeBytes( packet, 29 );
	}


	boost::thread( &ERobotCore::_armsTeachingThread, this );
}

void ERobotCore::stopArmsTeaching() {
	cout << "core :: stopArmsTeaching" << endl;
	isTeaching = false;

	if(depth_sensor_type.compare("kinect") != 0) { // xtion
		unsigned char packet[32] = {0x08, 0xF8, 0xF8, 0x0B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
		_bodyConnection->writeBytes( packet, 32 );
	} else {	// kinect
		unsigned char packet[29] = {0x08, 0xF8, 0xF8, 0x0B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
		_bodyConnection->writeBytes( packet, 29 );
	}


}

void ERobotCore::startHeadTeaching() {
	cout << "core :: startHeadTeaching" << endl;

	_bodyConnection->resetConnection();
		ros::Duration(1).sleep();

	if(depth_sensor_type.compare("kinect") != 0) { // xtion
		unsigned char packet[32] = {0x08, 0xF8, 0xF8, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
		_bodyConnection->writeBytes( packet, 32 );
	} else {	// kinect
		unsigned char packet[29] = {0x08, 0xF8, 0xF8, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
		_bodyConnection->writeBytes( packet, 29 );
	}

	boost::thread( &ERobotCore::_headTeachingThread, this );
}

void ERobotCore::_headTeachingThread() {
	int receivePacketSize			= 9;
	unsigned char* receivePacket = new unsigned char[receivePacketSize];

	stringstream teachingDataStream;
	ros::Duration bodyDelay = ros::Duration(0.1);

	std_msgs::String strMsg;
	isTeaching = true;
	while(isTeaching ){

		int readRealSize = _bodyConnection->readBytes(receivePacket,receivePacketSize);

		if(readRealSize == receivePacketSize) {
			char* buff = new char[10];
			for(int i=0 ; i<readRealSize ; i++) {
				sprintf(buff, "%02x ", receivePacket[i]);
				teachingDataStream << buff;
			}
			delete[] buff;
	//		cout << teachingDataStream.str() << endl;
			strMsg.data = teachingDataStream.str();
			_teachingPublisher.publish(strMsg);

			teachingDataStream.str("");
		}

		bodyDelay.sleep();
	}

	delete[] receivePacket;
}

void ERobotCore::_armsTeachingThread() {
	int receivePacketSize			= 17;
	unsigned char* receivePacket = new unsigned char[receivePacketSize];

	stringstream teachingDataStream;
	ros::Duration bodyDelay = ros::Duration(0.1);

	std_msgs::String strMsg;
	isTeaching = true;
	while(isTeaching ){

		int readRealSize = _bodyConnection->readBytes(receivePacket,receivePacketSize);

		char* buff = new char[10];
		for(int i=0 ; i<readRealSize ; i++) {
			sprintf(buff, "%02x ", receivePacket[i]);
			teachingDataStream << buff;
		}
		delete[] buff;
//		cout << teachingDataStream.str() << endl;
		strMsg.data = teachingDataStream.str();
		if(readRealSize == 17) {
			_teachingPublisher.publish(strMsg);
		}

		teachingDataStream.str("");

		bodyDelay.sleep();
	}

	delete[] receivePacket;
}

void ERobotCore::stopHeadTeaching() {
	cout << "core :: stopHeadTeaching" << endl;
	isTeaching = false;
	if(depth_sensor_type.compare("kinect") != 0) { // xtion
		unsigned char packet[32] = {0x08, 0xF8, 0xF8, 0xB0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
		_bodyConnection->writeBytes( packet, 32 );
	} else {	// kinect
		unsigned char packet[29] = {0x08, 0xF8, 0xF8, 0xB0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
		_bodyConnection->writeBytes( packet, 29 );
	}
}

void ERobotCore::startAutoCharge() {

	cout << "startAutoCharge!!!" << endl;

  isAutoCharging = true;
  _deltaMoveValue = 1;
  int sendPacketSize                              = 5;  // wheel encoder
  int encoderPacketSize= 4+9+2+1+1;

  unsigned char* sendPacket               = new unsigned char[sendPacketSize];
  memset(sendPacket, 0x00, sendPacketSize);
  sendPacket[0] = 0x03;
  sendPacket[1] = 0xf3;
  sendPacket[2] = 0xf3;
  sendPacket[3] = 0x0D;
  sendPacket[4] = 0x0D;

  _baseConnection->writeBytes( sendPacket, sendPacketSize );

  unsigned char* receivePacket = new unsigned char[encoderPacketSize];
  bool isTrue = false;
  while(!isTrue) {

  	int readRealSize = _baseConnection->readBytes(receivePacket,encoderPacketSize);

    printf("read size : %d \t",readRealSize);
    for(int i=0 ; i<readRealSize ; i++) {
    	printf("%02x ", receivePacket[i]);
    }
    printf("\n");


    isTrue = true;
    if (receivePacket[0] != 0x01) {
    	isTrue = false;
		} else if (receivePacket[1] != 0xF1) {
      isTrue = false;
    } else if (receivePacket[2] != 0xF1) {
      isTrue = false;
    } else {
      for(int i=3 ; i<15; i++) {
      	if(receivePacket[i] != 0xFF) {
        	isTrue = false;
          break;
        }
      }

    }
  	ros::Duration(0.1).sleep();
  }

  _deltaMoveValue = 88;
  isAutoCharging = false;


	cout << "finish autocharge!!!" << endl;
}


	void ERobotCore::stopAutoCharge() {

	cout << "stopAutoCharge!!!" << endl;

	isAutoCharging = true;
	_deltaMoveValue = 1;

	int sendPacketSize                              = 5;  // wheel encoder
	int encoderPacketSize= 4+9+2+1+1;

	unsigned char* sendPacket               = new unsigned char[sendPacketSize];
	memset(sendPacket, 0x00, sendPacketSize);
	sendPacket[0] = 0x03;
	sendPacket[1] = 0xf3;
	sendPacket[2] = 0xf3;
	sendPacket[3] = 0x11;
	sendPacket[4] = 0x11;

	_baseConnection->writeBytes( sendPacket, sendPacketSize );

	ros::Duration(1).sleep();

	_deltaMoveValue = 88;

  isAutoCharging = false;
  cout << "finish autocharge!!!" << endl;
}


void ERobotCore::getBodyPosition(double* pos) {
	for(int i=0; i<9; i++) {
		pos[i] = bodyPosition[i];
	}
}
