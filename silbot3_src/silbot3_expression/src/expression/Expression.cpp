/*
 * Expression.cpp
 *
 *  Created on: 2013. 4. 12.
 *      Author: root
 */

#include "Expression.h"


Expression::Expression() {
	runningFlag = false;
	_device_conn = NULL;
}


Expression::~Expression() {

}

void Expression::setExressionFlag(bool flag) {
	this->runningFlag = flag;
}

bool Expression::getRunningFlag() {
	return this->runningFlag;
}

void Expression::playWrongMessageSound() {

}


void Expression::setModality(const expression::modality::ModalityUseSet& modality) {
}

bool Expression::moveToHome(const ExpressionParameter& BehaviorInformation, const ModalityUseSet& modality) {

	ROS_DEBUG("Expression::moveToHome");

	if(_device_conn == NULL) {
		_device_conn = DeviceConnector::getInstance();
	}
	_device_conn->setInitData();

	int arm_speed = 30 ;
	double angle[3] = {0,0,0};
	if( modality.getNeck()) {
		_device_conn->getPantilt()->moveToHome(60);
	}
	if( modality.getArm1()) {
		_device_conn->getArm()->moveToPositionAll(0, angle, arm_speed);
	}
	if( modality.getArm2()) {
		_device_conn->getArm()->moveToPositionAll(1, angle, arm_speed);
	}
	if( modality.getWheel()) {
		_device_conn->getWheel()->stop();
	}
	return true;
}

	void Expression::executeDB(const ExpressionParameter& parameter, const ModalityUseSet& modality, ExpressionDB& db) {

		if(_device_conn == NULL) {
			_device_conn = DeviceConnector::getInstance();
		}

		const std::vector<ExpressionSequence>& ExpressionSequenceList  = db.getExpressionSequenceList();
		int len = ExpressionSequenceList.size();
		int i = 0;

		ROS_DEBUG("length : %d", len);
		double startTime = ros::Time::now().toSec();

		double ellapsedTime;
		double sleepTime;
		for ( i = 0 ; i < len && this->runningFlag; i++) {
			ellapsedTime = ros::Time::now().toSec() - startTime ;
			ExpressionSequence bs = ExpressionSequenceList.at(i);
			sleepTime = (bs.m_startTime - ellapsedTime);
			if(sleepTime >= 0.01) {
				ros::Duration(sleepTime).sleep();
			}
			ROS_DEBUG("play index : %d, sleepTime %f", i, sleepTime);
			for ( unsigned int listIndex = 0 ; listIndex < bs.m_modalityList.size() ; listIndex++ )	{
				ROS_DEBUG("execute db index i : %d , modal index : %d", i, listIndex );
				Modality mod = bs.m_modalityList.at(listIndex);

				executeModality(parameter, modality, mod);
				//			ROS_DEBUG("[%d] total : %d, current : %d", i, bs.m_modalityList.size(), listIndex);
			}
		}
		ROS_DEBUG("ExecuteDB out loop.... \n\n");
		moveToHome(parameter, modality);
		AvatarConnector::getInstance()->setMode("0");

		ROS_DEBUG("ExecuteDB FIN");
	}

	void Expression::executeModality(const ExpressionParameter& parameter, const ModalityUseSet& modalityUseSet,  Modality &mod )
	{

		if ( mod.ModalityName().compare("soundwav") == 0  )	{

			if( modalityUseSet.getSound()) {
				string soundName = mod.m_parameter["filename"];
				ROS_DEBUG("Expression::executeModality : %s", soundName.c_str() );
				try {

					SoundConnector::getInstance()->playSound(false, soundName);
				} catch(expression::exception::IOException& e) {

				}
			} else {
				ofLog::write(OF_LOG_WARNING, "Sound Modality OFF");
			}

		} else if ( mod.ModalityName().compare("ttswav") == 0  ) {

			// if( modalityUseSet.getTTS())	{
			//
			// } else {
			// 	ofLog::write(OF_LOG_WARNING, "TTS Modality OFF");
			// }

		} else if ( mod.ModalityName().compare("avatar") == 0  ) {

			if ( mod.FunctionName().compare("faceParam") == 0 )	{
				string face = mod.m_parameter["face"];
				string duration = mod.m_parameter["duration"];
				AvatarConnector::getInstance()->expression(face, duration);
			} else if ( mod.FunctionName().compare("playMocap") == 0 )	{
				string mocapFile = mod.m_parameter["mocapFile"];
//					string songDir = mod.m_parameter["songDir"];
//					string songFile = mod.m_parameter["songFile"];
//					AvatarConnector::getInstance()->playMocap(songDir, songFile, mocapDir, mocapFile);
				AvatarConnector::getInstance()->setMode("3");
			} else if ( mod.FunctionName().compare("start") == 0 )	{
				AvatarConnector::getInstance()->setMode("3");
			} else if ( mod.FunctionName().compare("stop") == 0 )	{
				AvatarConnector::getInstance()->setMode("0");
			}
		} else if ( mod.ModalityName().compare("pantilt") == 0 ) {
			if ( modalityUseSet.getNeck()) {
				if ( mod.FunctionName().compare("moveToAbsolutePosition") == 0 )	{

					double pan, tilt;
					int pantilt_speed;
					pan = ofUtil::ofToFloat(mod.m_parameter["pan"]);
					tilt = ofUtil::ofToFloat(mod.m_parameter["tilt"]);
					pantilt_speed = ofUtil::ofToInt(mod.m_parameter["speed"]);

					_device_conn->getPantilt()->moveToAbsolutePosition(  pan, tilt , pantilt_speed );

				}else if(mod.FunctionName().compare("moveToAbsolutePositionWithSpeed") == 0){
					double pan, tilt;
					int pan_speed, tilt_speed;
					pan = ofUtil::ofToFloat(mod.m_parameter["pan"]);
					tilt = ofUtil::ofToFloat(mod.m_parameter["tilt"]);
					pan_speed = ofUtil::ofToInt(mod.m_parameter["speedPan"]);
					tilt_speed = ofUtil::ofToInt(mod.m_parameter["speedTilt"]);

					_device_conn->getPantilt()->moveToAbsolutePositionWithSpeed(  pan, tilt , pan_speed, tilt_speed );

				}
			}else {
				ofLog::write(OF_LOG_WARNING, "NECK Modality OFF");
			}
		} else if ( mod.ModalityName().compare("wheel") == 0  ) {

			if( modalityUseSet.getWheel()) {
				if ( mod.FunctionName().compare("moveByWheelVelocity") == 0 ) {
					double leftWheelVelocity = ofUtil::ofToFloat( mod.m_parameter["leftWheelVelocity"] );
					double rightWheelVelocity = ofUtil::ofToFloat( mod.m_parameter["rightWheelVelocity"] );
					_device_conn->getWheel()->moveByWheelVelocity( leftWheelVelocity, rightWheelVelocity );

				} else if ( mod.FunctionName().compare("moveByVelocityXYT") == 0 ) {

					int currData[3];
					_device_conn->getCurrentData(currData);

					double velX = ofUtil::ofToFloat( mod.m_parameter["velX"] );
					double velY = ofUtil::ofToFloat( mod.m_parameter["velY"] );
					double velTheta = ofUtil::ofToFloat( mod.m_parameter["velTheta"] );

					int posX = ofUtil::ofToInt( mod.m_parameter["posX"] );
					int posY = ofUtil::ofToInt( mod.m_parameter["posY"] );
					int posT = ofUtil::ofToInt( mod.m_parameter["posT"] );

					if(posX != -1 && posY !=-1 && posT != -1) {
						int deltaX = _device_conn->getInitX() - currData[0] + posX;
						int deltaY = _device_conn->getInitY() - currData[1] + posY;
						int deltaT = _device_conn->getInitT() - currData[2] + posT;

						ROS_DEBUG("init : ( %d, %d, %d ) curr ( %d, %d, %d )",_device_conn->getInitX(),_device_conn->getInitY(),_device_conn->getInitT(),currData[0],currData[1],currData[2]);
						ROS_DEBUG("pos : ( %d, %d, %d ) delta ( %d, %d, %d )",posX,posY,posT,deltaX,deltaY,deltaT);
						ROS_DEBUG("vel : ( %f, %f, %f )",velX,velY,velTheta);
					// 	if(posX != -1) {
					// 		if(deltaX <= -1*xyBoundary || deltaX >= xyBoundary) {
					// 			velX = velX + (double)deltaX/wheelPeriod;
					// 		}
					// 	}
					//
					// 	if(posY != -1) {
					// 		if(deltaY <= -1*xyBoundary || deltaY >= xyBoundary) {
					// 			velY = velY + (double)deltaY/wheelPeriod;
					// 		}
					// 	}
					//
					// 	if(posT != -1) {
					// 		if(deltaT <= -1*tBoundary || deltaT >= xyBoundary) {
					// 			velTheta = velTheta - (double)deltaT/wheelPeriod;
					// 		}
					// 	}
					// }
				}
					ROS_DEBUG("result : ( %f, %f, %f )",velX,velY,velTheta);

					_device_conn->getWheel()->moveByVelocityXYT( velX, velY, velTheta);

				}  else if ( mod.FunctionName().compare("moveToDeltaXYTheta") == 0 ) {
					int deltaX = ofUtil::ofToInt( mod.m_parameter["deltaX"] );
					int deltaY = ofUtil::ofToInt( mod.m_parameter["deltaY"] );
					int deltaTheta = ofUtil::ofToInt( mod.m_parameter["deltaTheta"] );
					double velocity = ofUtil::ofToFloat( mod.m_parameter["velocity"] );
					//cout << "Expression 309 : moveToDeltaXYTheta : " << deltaX << " , " << deltaY << " , " <<deltaTheta << " , " <<velocity << endl;
					_device_conn->getWheel()->moveToDeltaXYTheta(deltaX, deltaY, deltaTheta, velocity);
				}

			}  else {
				// ofLog::write(OF_LOG_WARNING, "Wheel Modality OFF");
			}

		}

		else if ( mod.ModalityName().compare("arm") == 0 )
		{

			int armID = ofUtil::ofToInt( mod.m_parameter["armID"] );

			if( ( !modalityUseSet.getArm1()) && ( armID == 0 ) ) {
				// ofLog::write(OF_LOG_WARNING, "arm 0 Modality OFF");
				return ;
			}
			if( ( !modalityUseSet.getArm2()) && ( armID == 1 ) ) {
				// ofLog::write(OF_LOG_WARNING, "arm 1 Modality OFF");
				return ;
			}
			if ( mod.FunctionName().compare("moveToPositionAll") == 0 )	{
				double angle[3];
				angle[0] = ofUtil::ofToFloat( mod.m_parameter["angle[0]"] );
				angle[1] = ofUtil::ofToFloat( mod.m_parameter["angle[1]"] );
				angle[2] = ofUtil::ofToFloat( mod.m_parameter["angle[2]"] );

				double speed = ofUtil::ofToFloat( mod.m_parameter["speed"] );
				_device_conn->getArm()->moveToPositionAll(armID, angle, speed);
			}else if ( mod.FunctionName().compare("moveToPositionWithAxisSpeed") == 0 )	{
				double angle[3];
				angle[0] = ofUtil::ofToFloat( mod.m_parameter["angle[0]"] );
				angle[1] = ofUtil::ofToFloat( mod.m_parameter["angle[1]"] );
				angle[2] = ofUtil::ofToFloat( mod.m_parameter["angle[2]"] );

				double speed[3];
				speed[0]= ofUtil::ofToFloat( mod.m_parameter["speed[0]"] );
				speed[1]= ofUtil::ofToFloat( mod.m_parameter["speed[1]"] );
				speed[2]= ofUtil::ofToFloat( mod.m_parameter["speed[2]"] );
				_device_conn->getArm()->moveToPositionWithAxisSpeed(armID, angle, speed);

			} else if ( mod.FunctionName().compare("moveToHome") == 0 )	{
				int armID = ofUtil::ofToInt(mod.m_parameter["armID"]);
				ofLog::write(OF_LOG_VERBOSE, "moveToHome ArmID = %d", armID );
				_device_conn->getArm()->moveToHome(armID);
			} else if ( mod.FunctionName().compare("on") == 0 )	{
				int armID = ofUtil::ofToInt(mod.m_parameter["armID"]);
				ofLog::write(OF_LOG_VERBOSE, "moveToHome ArmID = %d", armID );
				_device_conn->getArm()->on(armID);
			}
		}
		else if ( mod.ModalityName().compare("led") == 0  )
		{
			if ( mod.FunctionName().compare("onRGB") == 0 )	{
				int id = ofUtil::ofToInt(mod.m_parameter["id"]);
				int blink =ofUtil::ofToInt(mod.m_parameter["blink"]);
				int red = ofUtil::ofToInt(mod.m_parameter["red"]);
				int green = ofUtil::ofToInt(mod.m_parameter["green"]);
				int blue = ofUtil::ofToInt(mod.m_parameter["blue"]);
					//				 _device_conn->getArm()->moveToHome(armID);
					//				 cout << "Expression 365 : mod.ModalityName().compare(\"led\") (" << id << " " << blink << " " << red << " " << green << " " << blue << " )" << endl;
					_device_conn->getLed()->on(id, (unsigned char)blink, (unsigned char)red, (unsigned char)green, (unsigned char)blue);
			}
		}
		else if ( mod.ModalityName().compare("position") == 0  ) {

			if( modalityUseSet.getWheel()) {
				if ( mod.FunctionName().compare("setPosition") == 0 ) {
					int x = ofUtil::ofToInt( mod.m_parameter["x"] );
					int y = ofUtil::ofToInt( mod.m_parameter["y"] );
					int t = ofUtil::ofToInt( mod.m_parameter["t"] );
					int s = ofUtil::ofToInt( mod.m_parameter["speed"] );
					int currData[3] = {0,0,0};
					_device_conn->getCurrentData(currData);
					ROS_DEBUG("init position (%d, %d, %d)",_device_conn->getInitX(), _device_conn->getInitY(), _device_conn->getInitT());
					ROS_DEBUG("position setPosition (%d, %d, %d, %d)",x, y, t, s);
					int deltaX = _device_conn->getInitX() - currData[0] + x;
					int deltaY = _device_conn->getInitY() - currData[1] + y;
					int deltaT = _device_conn->getInitT() - currData[2] + t;
					if(x == -1) {
						deltaX = 0;
					}
					if(y == -1) {
						deltaY = 0;
					}
					if(t == -1) {
						deltaT = 0;
					}
					if(s <= 0) {
						s = 80;
					}
					ROS_DEBUG("position moveToDeltaXYTheta result (%d, %d, %d, %d)",deltaX, deltaY, deltaT, s);
					_device_conn->getWheel()->moveToDeltaXYTheta( deltaX, deltaY, deltaT, s);
				}
			}
		}
	}
