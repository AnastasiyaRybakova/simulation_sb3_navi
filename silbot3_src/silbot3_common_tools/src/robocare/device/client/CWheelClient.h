#ifndef __ROSCIR_CLIENT_WHEEL_H__
#define __ROSCIR_CLIENT_WHEEL_H__

#include <robocare/device/common/Wheel.h>
#include <robocare/device/RosCIRDefinitions.h>

#include <iostream>

#include <ros/ros.h>
#include <silbot3_msgs/Device_Wheel_Srv.h>
#include <silbot3_msgs/Device_Wheel_Msg.h>
#include <silbot3_msgs/Device_Error_Srv.h>

#include <robocare/device/client/DeviceClient.h>
#include <boost/thread.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>

using namespace std;

class CIRAPI CWheelClientProxy
{
    private :
        CWheelClientProxy();

    public :
        virtual ~CWheelClientProxy();
        static CWheel* newInstance();
};


class CWheelROSStub : public CWheel
{
    private:
		void close();
		ros::NodeHandle			_nodeHandle;
		ros::ServiceClient		_wheelClient;
		ros::ServiceClient		_wheelErrorClient;
		ros::Publisher			_wheelPublisher;
		ros::Publisher 			_twistPublisher;
		ros::Subscriber			_wheelSubscriber;
		int deltaMoveResult;
		boost::condition_variable _condition;
		boost::mutex _mutex;
		bool isDeltaMove;

    public :
		CWheelROSStub();
        virtual ~CWheelROSStub();

        void subscribeCallBack(const silbot3_msgs::Device_Wheel_Msg::ConstPtr& message);

        // Overriding
        int on();
        int off();
        int stop();
        int moveByTRVelocity( double translationalVelocity, double rotationalVelocity );
        int moveByWheelVelocity( double leftWheelVelocity, double rightWheelVelocity );
        int moveToDeltaX( int deltaX, double velocity );
        int moveToDeltaTheta( int deltaTheta, int radius, double velocity );
        int moveToDeltaXYTheta( int deltaX, int deltaY, int deltaTheta, double velocity );
        int waitmoveToDeltaFinished();
        int moveByVelocityXYT(double velX, double velY, double velTheta);

		int moveByWheelVelocity( double wheel1Rpm, double wheel2Rpm, double wheel3Rpm);
		int reset();
		int moveByTwistMsg(const geometry_msgs::Twist& twistMsg);
		int emergencyStop();
		int emergencyRelease();
		int emergencyMoveByVelocityXYT(double velX, double velY, double velTheta);
		int selfDiagnosis(short errorCode[17]);

    int startAutoCharge() {}
    int stopAutoCharge() {}
};

#endif /* __ROSCIR_CLIENT_WHEEL_H__ */
