#ifndef __EXPRESSION_CONNECTOR_DEVICE_CONNECTOR_H__
#define __EXPRESSION_CONNECTOR_DEVICE_CONNECTOR_H__

#define M_PI       3.14159265358979323846

#include <robocare/device/client/CArmClient.h>
#include <robocare/device/client/CWheelClient.h>
#include <robocare/device/client/CPanTiltClient.h>
#include <robocare/device/client/CLightEmittingDiodeClient.h>
#include <robocare/device/client/CGyroClient.h>
#include <robocare/device/client/CWheelEncoderClient.h>

using namespace std;

namespace expression {
	namespace connector {

			class DeviceConnector {
			private:

				CPanTilt*	m_panTilt;
				CArm*		m_arm;
				CWheel*		m_wheel;
				CLightEmittingDiode*	m_led;
				CGyro*		m_gyro;
				CWheelEncoder*	m_encoder;
				int initX;
				int initY;
				int initT;

			public:
				DeviceConnector();
				virtual ~DeviceConnector();

				static DeviceConnector* getInstance();

				CArm* getArm();
				CPanTilt* getPantilt();
				CWheel* getWheel();
				CLightEmittingDiode* getLed();
				CGyro* getGyro();
				CWheelEncoder* getEncoder();

				void setInitData();
				int getInitX();
				int getInitY();
				int getInitT();
				void getCurrentData(int* data);
			};

	} /* namespace player */
} /* namespace expression */
#endif /* __EXPRESSION_CONNECTOR_DEVICE_CONNECTOR_H__ */
