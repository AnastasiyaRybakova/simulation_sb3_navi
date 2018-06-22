#ifndef __EROBOT_CORE_H__
#define __EROBOT_CORE_H__

#include <stdio.h>
#include <iostream>

#include <boost/thread.hpp>
#include <pthread.h>

#include <ros/ros.h>
#include <reactive/erobot/session/ERobotSerialConnector.h>
#include <reactive/erobot/utility/ERobotFunctionFactory.h>
#include <reactive/erobot/session/EWheelEncoderCore.h>
#include <reactive/erobot/session/EUltraCore.h>
#include <reactive/erobot/session/ETouchCore.h>
#include <reactive/erobot/session/EWheelCore.h>
#include <reactive/erobot/session/EArmCore.h>
#include <reactive/erobot/session/EPantiltCore.h>
#include <reactive/erobot/session/ELEDCore.h>
#include <reactive/erobot/session/EBatteryCore.h>
#include <reactive/erobot/session/EBodyCoreKinect.h>
#include <reactive/erobot/session/EBodyCoreXtion.h>
#include <silbot3_msgs/Device_Wheel_Msg.h>
#include <std_msgs/String.h>

#include <reactive/erobot/utility/ERobotBlockingQueue.h>
#include <reactive/erobot/utility/ERobotQueue.h>

using namespace std;

namespace reactive {
	namespace erobot {
		namespace core {
			class ERobotCore {

			private :

				bool _ISEND;
				bool isTest;
				bool isBatteryCheck;
				string depth_sensor_type;

				static pthread_mutex_t __comunicate_mutex;
				static pthread_mutex_t __body_mutex;
				static pthread_mutex_t __sensor_mutex;
				static pthread_cond_t __cv;

				ros::NodeHandle	_nodehandle;
				bool _isDeltaMove;
				bool _isArmUpdate;
				bool _isPantiltUpdate;
				bool _isLedUpdate;
				ros::Publisher _deltaPublisher;
				ros::Publisher _teachingPublisher;

				bool isUltraError;
				bool isGyroError;
				bool isErrorCheckEnd;
				bool isTouchErrorChecking;

				bool isTeaching;
				bool isAutoCharging;

				bool _isWheelUpdate;
				int _deltaMoveValue;
				int _oldEncoderCount;

				EWheelEncoderCore*		_wheel_encoder_core;
				EUltraCore*				_ultrasonic_core;
				ETouchCore*				_touch_core;
				EWheelCore*				_wheel_core;
				EPantiltCore*			_pantilt_core;
				EArmCore*				_arm_core;
				ELEDCore*				_led_core;
				EBatteryCore*			_battery_core;
				EBodyCore*				_body_core;

				int helper;

				double bodyPosition[9];

			private :
				ERobotSerialConnector* _baseConnection;
				ERobotSerialConnector* _bodyConnection;
				ERobotSerialConnector* _sensorConnection;
				ERobotSerialConnector* _etcConnection;


				ERobotQueue<short*>	_leftArmQueue;
				ERobotQueue<short*>	_rightArmQueue;
				ERobotQueue<unsigned char*>	_pantiltQueue;

				void _updateUltrasonic();
				void _updateWheel();
				void _updateArm();
				void _updatePantilt();
				void _updateTouchButton();
				void _updateLED();
				void _updateMotorError();
				void _updateWheelEncoder();
				void _sendBaseResponse();

				void _checkUltrasonicGyroError();
				void _checkWheelErrorTimeover();

			public :
				ERobotCore();
				virtual ~ERobotCore();

				static ERobotCore* getInstance();

//				static void CALLBACK TimeProc(UINT m_nTimerID, UINT uiMsg, DWORD dwUser, DWORD dw1, DWORD d2);
				void initERobotCore();
				void checkMotorError();

				void checkVersion();
				string checkVersionWheel();
				string checkVersionArm();
				string checkVersionHead();
				string checkVersionUltra();
				string checkVersionSensors();

				void _updateBase();
				void _updateBody();
				void _updateSensor();
				void _updateEtc();
				void _updateBodyPosition();
				void getWheelEncoderValue(long* data, int* x, int* y, int* t );
				void getUltrasonicValue(unsigned short *data, int nDataSize);
				void getTouchButtonValue(unsigned short *data, int nDataSize);
				void getBatteryValue(int* voltage);
				void getArmPosition(short* positions);
				void getPantiltPosition(double* positions);

				int setWheelValue(unsigned char command, int param1, int param2, int param3, int param4);
				int setPanTiltValue(unsigned char id, short anglePan, short angleTilt, short speedPan, short speedTilt);
				int setXtionPanTiltValue(unsigned char id, short anglePan, short angleTilt, short angleXtion, short speedPan, short speedTilt, short speedXtion);
				int setLEDValue(unsigned char id, unsigned char red, unsigned char green, unsigned char blue, unsigned char blink);
				int setArmValue(unsigned char command, short *angles, short *speeds);
				int setArmHomePosition();

				void selfDiagnosisWheel(short errorCode[9]);
				void selfDiagnosisWheelNoMove(short errorCode[9]);
				void errorDetectingPantilt(short errorCode[3]);
				void errorDetectingArm(short errorCode[6]);
				int errorDetectingTouch(int touchID);


				void viewMessageBox(const char* message, const char* title);
				void waitForWheelUpdate();

				void releaseTorue();
				void startArmsTeaching();
				void stopArmsTeaching();
				void startHeadTeaching();
				void stopHeadTeaching();
				void _headTeachingThread();
				void _armsTeachingThread();


				void startAutoCharge();
				void stopAutoCharge();
				void getBodyPosition(double* pos);

			};
		}
	}
}

#endif /* __EROBOT_CORE_H__ */
