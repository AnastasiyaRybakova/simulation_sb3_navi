#include "ExpressionDBLoader.h"
#include <ros/ros.h>

namespace expression {
namespace db {

bool CompareObj( ExpressionSequence first, ExpressionSequence second ) {
	return first.m_startTime < second.m_startTime;
}

ExpressionDBLoader::ExpressionDBLoader() {
	this->DBLoaded = false;
	this->_songDir = "/opt/robocare/common/sound/song";
	this->_expressionSoundDir =  "/opt/robocare/common/sound/effect";
	this->_songName = "ready.wav";
	this->_songTime = 0.0;
}

ExpressionDBLoader::~ExpressionDBLoader() {
}

string ExpressionDBLoader::getSongDir() {
	return this->_songDir;
}
string ExpressionDBLoader::getSongName() {
	return this->_songName;
}

double ExpressionDBLoader::getSongTime() {
	return this->_songTime;
}

bool ExpressionDBLoader::loadExpressionDBFromFile( string filename, ExpressionDB& db ) {
	DBLoaded = false;

	expressionSequenceList.clear();

	if( access(filename.c_str(), 0) == -1) {
		string msg = filename +" file name xml does not exist. Default DB Loaded";
		filename = "/opt/robocare/silbot3/expression/expression_avatar_speak_short.xml";
	}

	ROS_INFO("ExpressionDBLoader. try to load Expression db file [%s]", filename.c_str());
	bool result = this->XML.loadFile(filename);

	if ( result ) {
		ROS_INFO("xml loaded!: [%s]", filename.c_str());
	} else {
		ROS_ERROR("Cannot Load: [%s]", filename.c_str());
		return false;
	}

	int numDragTags = XML.getNumTags("DB:start");
	if(numDragTags > 0){
		XML.pushTag("DB", numDragTags - 1 );
		int numStartTags = XML.getNumTags("start");
		if(numStartTags > 0) {
			for ( int i = 0 ; i < numStartTags ; i++) {
				loadExpressionSequence(db, i);
			}
		}
		modityStartTimeEndTime();
		setSpeakTime();
		std::sort(expressionSequenceList.begin(), expressionSequenceList.end(), CompareObj);
	}
	db.setExpressionSequenceList(this->expressionSequenceList);
	DBLoaded = true;
	return true;
}


void ExpressionDBLoader::loadExpressionSequence(ExpressionDB& db,  int id, double offsetTime)	{
	ExpressionSequence bs;
	double time;
	time = XML.getAttribute("start", "time", -1.0, id);

	bs.m_startTime = time + offsetTime;
	XML.pushTag("start",id);
	int numDeviceTags = XML.getNumTags("device");

	for ( int i = 0 ; i < numDeviceTags ; i++)	{
		string device_name = XML.getAttribute("device", "name", "NO_DEVICE", i);
		string method = XML.getAttribute("device", "method", "NO_METHOD", i);
//		cout << i << " ]] device_name : " << device_name << " , method : " << method << endl;
		if ( device_name.compare( "soundwav" ) == 0 ) {
//			cout << "ExpressionDBLoader 94 : soundwav " << endl;
			XML.pushTag("device",i);
			loadSoundWav(bs, db);
			XML.popTag();

		} else if ( device_name.compare( "ttswav" ) == 0 )  {
			XML.pushTag("device",i);
			loadPlayTTS(bs);
			XML.popTag();
		} else if ( device_name.compare( "avatar" ) == 0 ) {
			XML.pushTag("device",i);

			if( method.compare("mpExpress") == 0 ) {			// engkey
				loadEngkeyAvatarExpress(bs);
			} else if( method.compare("mpPlaySongLipsync") == 0 ) {				// engkey
				loadEngkeyAvatarPlaySongLipsync(bs);
			} else if( method.compare("mpExpressHome") == 0 ) {					// engkey
				loadEngkeyAvatarMpExpressEmotion(bs, "mpExpressHome");
			} else if( method.compare("mpExpressAngry") == 0 ) {				// engkey
				loadEngkeyAvatarMpExpressEmotion(bs, "mpExpressAngry");
			} else if( method.compare("mpExpressFunny") == 0 ) {				// engkey
				loadEngkeyAvatarMpExpressEmotion(bs, "mpExpressFunny");
			} else if( method.compare("mpExpressHappy") == 0 ) {				// engkey
				loadEngkeyAvatarMpExpressEmotion(bs, "mpExpressHappy");
			} else if( method.compare("mpExpressSad") == 0 ) {					// engkey
				loadEngkeyAvatarMpExpressEmotion(bs, "mpExpressSad");
			} else if( method.compare("mpExpressSurprize") == 0 ) {				// engkey
				loadEngkeyAvatarMpExpressEmotion(bs, "mpExpressSurprize");
			} else if( method.compare("mpExpressWink") == 0 ) {					// engkey
				loadEngkeyAvatarMpExpressEmotion(bs, "mpExpressWink");
			} else if( method.compare("mpExpressSmile") == 0 ) {				// engkey
				loadEngkeyAvatarMpExpressEmotion(bs, "mpExpressSmile");
			} else if( method.compare("mpLookAt") == 0 ) {						// engkey
				loadEngkeyAvatarLookAt(bs);
			} else if( method.compare("mpCloseEye") == 0 ) {					// engkey
				loadEngkeyAvatarCloseEye(bs);
			} else if( method.compare("mpBlink") == 0 )	{						// engkey
				loadEngkeyAvatarBlink(bs);
			} else if( method.compare("mpSetPupilPos") == 0 ) {					// engkey
				loadEngkeyAvatarPupilPos(bs);
			} else if( method.compare("mpSetEyeClose") == 0 ) {					// engkey
				loadEngkeyAvatarSetEyeClose(bs);
			} else if( method.compare("faceParam") == 0 ) {						// silbot3
				loadAvatarFaceParam(bs);
			} else if( method.compare("playMocap") == 0 ) {						// silbot3
				loadAvatarPlayMocap(bs);
			} else if( method.compare("start") == 0 ) {						// silbot3
				Modality mod("avatar", "start");

				bs.m_modalityList.push_back(mod);
			} else if( method.compare("stop") == 0 ) {						// silbot3
				Modality mod("avatar", "stop");

				bs.m_modalityList.push_back(mod);
			}
			XML.popTag();
		} else if ( device_name.compare( "wheel" ) == 0 ) {
			XML.pushTag("device",i);

			if( method.compare("moveByWheelVelocity") == 0 ) {					// engkey
				loadMoveByWheelVelocity(bs);
			} else if( method.compare("moveByVelocityXYT") == 0 ) {				// engkey
				loadMoveByVelocityXYT(bs);
			}  else if( method.compare("moveToDeltaXYTheta") == 0 ) {				// engkey
				loadMoveToDeltaXYTheta(bs);
			}
			XML.popTag();
		} else if ( ( device_name.compare( "neck" ) == 0  )
				|| (device_name.compare( "pantilt" ) == 0)
				|| (device_name.compare( "rollbend" ) == 0)) {

			XML.pushTag("device",i);

			if( method.compare("moveToAbsolutePosition") == 0 )	{
				loadEngkeyKiboNeck(bs);
			}else if( method.compare("moveToAbsolutePositionWithSpeed") == 0 )	{
				loadERobotNeck(bs);
				//									cout << "device_name: " << device_name << " moveToPositionWithAxisSpeed method: " << method << endl;
			}
			XML.popTag();

		} else if ( device_name.compare( "arm" ) == 0 ) {
			XML.pushTag("device",i);
			loadEngkeyArm(bs, method);
			XML.popTag();

		} else if ( device_name.compare( "led" ) == 0 ) {
//			cout << "ExpressionDBLoader 200 : load led" << endl;
			XML.pushTag("device",i);
			loadLedExpression(bs);
			XML.popTag();
		} else if ( device_name.compare( "position" ) == 0 ) {
			//			cout << "ExpressionDBLoader 200 : load led" << endl;
			XML.pushTag("device",i);
			loadPosition(bs);
			XML.popTag();
		}
	}
	XML.popTag();
	expressionSequenceList.push_back(bs);
}


void ExpressionDBLoader::modityStartTimeEndTime() {

}

bool ExpressionDBLoader::findSoundModality() {
	bool res = false;

	for (int i = 0 ; i < expressionSequenceList.size() ; i++)
	{
		int len = expressionSequenceList.at(i).m_modalityList.size();

		for ( int j = 0 ; j < len ; j++)
		{
			if ( expressionSequenceList.at(i).m_modalityList.at(j).ModalityName().compare("soundwav") == 0 )
			{
				return true;
			}
		}
	}

	return res;
}

bool ExpressionDBLoader::loadSoundWav( ExpressionSequence &bs, ExpressionDB& db ) {
	int numParameterTags = XML.getNumTags("parameter");
	string fileName;

//	cout << "ExpressionDBLoader 239 : numParameterTags : " << numParameterTags << endl;
	for ( int k = 0 ; k < numParameterTags ; k++) {
		string parameterName = XML.getAttribute("parameter", "name", "PARAMETER_NAME", k );
		if ( parameterName.compare("filename") == 0 ) {
			fileName = XML.getAttribute("parameter", "value", "0", k );
		}
	}

	//ofLog::write(OF_LOG_WARNING, "PlaySongLipsync songName = %s", songName.c_str() );

	Modality mod("soundwav", "playsound");
//	cout << "ExpressionDBLoader 250 : playsound : " << fileName << endl;

	mod.addParameter("filename", fileName);

	bs.m_modalityList.push_back(mod);
	return true;

}

bool ExpressionDBLoader::loadPlayTTS( ExpressionSequence &bs ) {
	int numParameterTags = XML.getNumTags("parameter");

	for ( int k = 0 ; k < numParameterTags ; k++) {

		string parameterName = XML.getAttribute("parameter", "value", "PARAMETER_NAME", k );

		if ( parameterName.compare("StartLipSync") == 0 ) {
			Modality mod("ttswav", "playtts");
			bs.m_modalityList.push_back(mod);
		}

	}
	return true;
}

bool ExpressionDBLoader::loadEngkeyFace( ExpressionSequence &bs )
{
	//DBLoaded = false;
	int numParameterTags = XML.getNumTags("parameter");
	string duration, faceNum, weight;

	for ( int k = 0 ; k < numParameterTags ; k++) {

		string parameterName = XML.getAttribute("parameter", "name", "PARAMETER_NAME", k );

		if ( parameterName.compare("duration") == 0 ) {
			duration = XML.getAttribute("parameter", "value", "0", k );
		} else if ( parameterName.compare("faceNum") == 0 )	{
			faceNum = XML.getAttribute("parameter", "value", "0", k );
		} else if ( parameterName.compare("weight") == 0 ) {
			weight = XML.getAttribute("parameter", "value", "0", k );
		} else {
			return false;
		}
	}

	//ofLog::write(OF_LOG_VERBOSE, "duration = %s, faceNum = %s, weight = %s", duration.c_str(), faceNum.c_str(), weight.c_str());

	Modality mod("face", "moveAbsolutePosition");
	mod.addParameter("duration", duration);
	mod.addParameter("faceNum", faceNum);
	mod.addParameter("weight", weight);

	bs.m_modalityList.push_back(mod);

	return true;
}

bool ExpressionDBLoader::loadEngkeyAvatarExpress( ExpressionSequence &bs )
{
	//DBLoaded = false;

	int numParameterTags = XML.getNumTags("parameter");

	string duration, faceNum, weight;

	for ( int k = 0 ; k < numParameterTags ; k++) {

		string parameterName = XML.getAttribute("parameter", "name", "PARAMETER_NAME", k );

		if ( parameterName.compare("duration") == 0 ) {
			duration = XML.getAttribute("parameter", "value", "0", k );
		} else {
			size_t found;

			found=parameterName.find("value");
			if (found!=string::npos) {
				faceNum=parameterName.erase(0,5);
				weight = XML.getAttribute("parameter", "value", "0", k );
			}
		}
	}

	//ofLog::write(OF_LOG_NOTICE, "duration = %s, faceNum = %s, weight = %s", duration.c_str(), faceNum.c_str(), weight.c_str());

	Modality mod("avatar", "mpExpress");
	mod.addParameter("duration", duration);
	mod.addParameter("faceNum" , faceNum);
	mod.addParameter("weight", weight);

	bs.m_modalityList.push_back(mod);
	return true;
}

bool ExpressionDBLoader::loadEngkeyAvatarPlaySongLipsync( ExpressionSequence &bs ) {

	int numParameterTags = XML.getNumTags("parameter");
	string songName;

	for ( int k = 0 ; k < numParameterTags ; k++) {

		string parameterName = XML.getAttribute("parameter", "name", "PARAMETER_NAME", k );
		if ( parameterName.compare("songName") == 0 ) {
			songName = XML.getAttribute("parameter", "value", "0", k );
		}
	}

	//ofLog::write(OF_LOG_WARNING, "PlaySongLipsync songName = %s", songName.c_str() );

	Modality mod("avatar", "mpPlaySongLipsync");
	mod.addParameter("songName", songName);
	_songTime = bs.m_startTime;

	bs.m_modalityList.push_back(mod);

	// this->_songName = "";
	// this->_songDir = "";
	// std::vector<std::string> strs;
	// boost::split(strs, songName, boost::is_any_of("/"));
	// int size = strs.size();
	//
	// bool songdir = false;
	// this->_songName = strs.at(size-1);
	// for(int i=0; i< size-1; i++) {
	// 	if(strs.at(i) == "song") {
	// 		this->_songDir = "";
	// 		songdir = true;
	// 		continue;
	// 	}
	// 	if(songdir) {
	// 		this->_songDir += "/" + strs.at(i);
	// 		//this->_songDir += strs.at(i);
	// 	}
	// }
	//
	// stringstream ss;
	//
	// if(_songDir.empty()){
	// 	ss<<"/opt/robocare/common/sound/song";
	// }else{
	// 	ss<<"/opt/robocare/common/sound/song"<<_songDir;
	// 	//ss<<_songDir;
	// }



	this->_songDir = "";
	this->_songName = songName;

	//			cout << "sonudDir "<< this->_songDir << endl;
	//			cout << "songname " << this->_songName << endl;
	return true;
}


bool ExpressionDBLoader::loadEngkeyKiboNeck( ExpressionSequence &bs ) {

	int numParameterTags = XML.getNumTags("parameter");
	string str_degreeUpDown, str_degreeLeftRight, str_speed;
	double degreeUpDown, degreeLeftRight, speed;

	for ( int k = 0 ; k < numParameterTags ; k++) {
		string parameterName = XML.getAttribute("parameter", "name", "PARAMETER_NAME", k );

		if ( parameterName.compare("degreeUpDown") == 0 ) {
			str_degreeUpDown = XML.getAttribute("parameter", "value", "0", k );
		} else if ( parameterName.compare("degreeLeftRight") == 0 ) {
			str_degreeLeftRight = XML.getAttribute("parameter", "value", "0", k );
		} else if ( parameterName.compare("speed") == 0 ) {
			str_speed = XML.getAttribute("parameter", "value", "0", k );
		} else {
			return false;
		}
	}

	//ofLog::write(OF_LOG_VERBOSE, "degreeUpDown = %s, degreeLeftRight = %s, speed = %s", str_degreeLeftRight.c_str(), str_degreeUpDown.c_str(), str_speed.c_str());
	Modality mod("pantilt", "moveAbsolutePosition");

	mod.addParameter("pan", str_degreeLeftRight);
	mod.addParameter("tilt", str_degreeUpDown);
	mod.addParameter("speed", str_speed);

	bs.m_modalityList.push_back(mod);

	return true;
}

bool ExpressionDBLoader::loadERobotNeck( ExpressionSequence &bs ){
	int numParameterTags = XML.getNumTags("parameter");
	string str_degreeUpDown, str_degreeLeftRight,  str_speedUpDown, str_speedLeftRight;
	for ( int k = 0 ; k < numParameterTags ; k++) {
		string parameterName = XML.getAttribute("parameter", "name", "PARAMETER_NAME", k );
		if ( parameterName.compare("degreeTilt") == 0 ) {
			str_degreeUpDown = XML.getAttribute("parameter", "value", "0", k );
		} else if ( parameterName.compare("degreePan") == 0 )	{
			str_degreeLeftRight = XML.getAttribute("parameter", "value", "0", k );
		} else if ( parameterName.compare("speedTilt") == 0 ) {
			str_speedUpDown = XML.getAttribute("parameter", "value", "0", k );
		} else if ( parameterName.compare("speedPan") == 0 ) {
			str_speedLeftRight = XML.getAttribute("parameter", "value", "0", k );
		}else {
			return false;
		}
	}

	Modality mod("pantilt", "moveToAbsolutePositionWithSpeed");
	mod.addParameter("pan", str_degreeLeftRight);
	mod.addParameter("tilt", str_degreeUpDown);
	mod.addParameter("speedTilt", str_speedUpDown);
	mod.addParameter("speedPan", str_speedLeftRight);

	bs.m_modalityList.push_back(mod);
	return true;
}

bool ExpressionDBLoader::loadLedExpression( ExpressionSequence &bs ) {
//	cout << "ExpressionDBLoader 700 : loadLedExpression " << endl;
	int numParameterTags = XML.getNumTags("parameter");

	string id,blink,red,green,blue;

	for ( int k = 0 ; k < numParameterTags ; k++) {
		string parameterName = XML.getAttribute("parameter", "name", "PARAMETER_NAME", k );

		if ( parameterName.compare("blink") == 0 )	{
			blink = XML.getAttribute("parameter", "value", "0", k );
		} else if ( parameterName.compare("red") == 0 )	{
			red = XML.getAttribute("parameter", "value", "0", k );
		} else if ( parameterName.compare("green") == 0 ) {
			green = XML.getAttribute("parameter", "value", "0", k );
		}else if ( parameterName.compare("blue") == 0 ) {
			blue = XML.getAttribute("parameter", "value", "0", k );
		} else if ( parameterName.compare("id") == 0 )	{
			id = XML.getAttribute("parameter", "value", "0", k );
		}  else {
			return false;
		}

	}

	//ofLog::write(OF_LOG_VERBOSE, "UpperFlag = %s, LegFlag = %s, Expression = %s, MotionNum = %s", UpperFlag.c_str(), LegFlag.c_str(),Expression.c_str() ,  MotionNum.c_str());

//	cout << "ExpressionDBLoader 726 : loadLedExpression (" << id << " " << blink << " " << red << " " << green <<" " << blue << ") " << endl;
	Modality mod("led", "onRGB");

	mod.addParameter("id", id);
	mod.addParameter("blink", blink);
	mod.addParameter("red", red);
	mod.addParameter("green", green );
	mod.addParameter("blue", blue);

	bs.m_modalityList.push_back(mod);

	return true;
}

bool ExpressionDBLoader::loadMoveByVelocityXYT( ExpressionSequence &bs )
{
	int numParameterTags = XML.getNumTags("parameter");

	string velX, velY, velTheta, posX, posY, posT;

	posX = "-1";
	posY = "-1";
	posT = "-1";

	for ( int k = 0 ; k < numParameterTags ; k++) {

		string parameterName = XML.getAttribute("parameter", "name", "PARAMETER_NAME", k );

		if ( parameterName.compare("velX") == 0 ) {
			velX = XML.getAttribute("parameter", "value", "0", k );
		} else if ( parameterName.compare("velY") == 0 ) {
			velY = XML.getAttribute("parameter", "value", "0", k );
		} else if ( parameterName.compare("velTheta") == 0 ) {
			velTheta = XML.getAttribute("parameter", "value", "0", k );
		} else if ( parameterName.compare("posX") == 0 ) {
			posX = XML.getAttribute("parameter", "value", "0", k );
		} else if ( parameterName.compare("posY") == 0 ) {
			posY = XML.getAttribute("parameter", "value", "0", k );
		} else if ( parameterName.compare("posT") == 0 ) {
			posT = XML.getAttribute("parameter", "value", "0", k );
		} else {
			continue;
		}
	}
	Modality mod("wheel", "moveByVelocityXYT");

	mod.addParameter("velX",  velX);
	mod.addParameter("velY", velY );
	mod.addParameter("velTheta", velTheta );
	mod.addParameter("posX", posX );
	mod.addParameter("posY", posY );
	mod.addParameter("posT", posT );

	bs.m_modalityList.push_back(mod);

	return true;
}

bool ExpressionDBLoader::loadMoveToDeltaXYTheta( ExpressionSequence &bs )
{
	int numParameterTags = XML.getNumTags("parameter");

	string velX, velY, velTheta, velocity;

	for ( int k = 0 ; k < numParameterTags ; k++) {

		string parameterName = XML.getAttribute("parameter", "name", "PARAMETER_NAME", k );

		if ( parameterName.compare("deltaX") == 0 ) {
			velX = XML.getAttribute("parameter", "value", "0", k );
		} else if ( parameterName.compare("deltaY") == 0 ) {
			velY = XML.getAttribute("parameter", "value", "0", k );
		} else if ( parameterName.compare("deltaTheta") == 0 ) {
			velTheta = XML.getAttribute("parameter", "value", "0", k );
		} else if ( parameterName.compare("velocity") == 0 ) {
			velocity = XML.getAttribute("parameter", "value", "0", k );
		} else {
			return false;
		}
	}
	Modality mod("wheel", "moveToDeltaXYTheta");

	mod.addParameter("deltaX", velX );
	mod.addParameter("deltaY", velY );
	mod.addParameter("deltaTheta", velTheta );
	mod.addParameter("velocity", velocity );


	bs.m_modalityList.push_back(mod);

	return true;
}

bool ExpressionDBLoader::loadMoveByWheelVelocity( ExpressionSequence &bs ) {

	int numParameterTags = XML.getNumTags("parameter");
	string leftWheelVelocity, rightWheelVelocity;

	for ( int k = 0 ; k < numParameterTags ; k++) {

		string parameterName = XML.getAttribute("parameter", "name", "PARAMETER_NAME", k );

		if ( parameterName.compare("leftWheelVelocity") == 0 ) {
			leftWheelVelocity = XML.getAttribute("parameter", "value", "0", k );
		} else if ( parameterName.compare("rightWheelVelocity") == 0 ) {
			rightWheelVelocity = XML.getAttribute("parameter", "value", "0", k );
		} else {
			return false;
		}
	}

	Modality mod("wheel", "moveByWheelVelocity");

	mod.addParameter("leftWheelVelocity", leftWheelVelocity );
	mod.addParameter("rightWheelVelocity", rightWheelVelocity );

	bs.m_modalityList.push_back(mod);

	return true;
}


bool ExpressionDBLoader::loadEngkeyArm( ExpressionSequence &bs , string ms) {

	int numParameterTags = XML.getNumTags("parameter");

	if ( ms.compare("moveToPositionAll") == 0 ) {
		engkeyArm3DOF(numParameterTags, bs);
	} else if ( ms.compare("moveToPositionWithAxisSpeed") == 0 ) {
		eRobotArm3DOF( numParameterTags, bs );
	}else if ( ms.compare("moveToHome") == 0 ) {
		string armID ;
		for ( int k = 0 ; k < numParameterTags ; k++) {
			string parameterName = XML.getAttribute("parameter", "name", "PARAMETER_NAME", k );
			if ( parameterName.compare("armID") == 0 ) {
				armID = XML.getAttribute("parameter", "value", "0", k );
			} else {
				string tmp = "NOT WANTED ARM PARAMETER";
				tmp = tmp + ofUtil::ofToString(bs.m_startTime, 4);

				//ofLog::write(OF_LOG_ERROR, tmp.c_str());
				abort();
				return false;
			}
		}
		//ofLog::write(OF_LOG_VERBOSE, "moveToHome %s ", armID.c_str() );
		Modality mod("arm", "moveToHome");
		mod.addParameter("armID", armID );

		bs.m_modalityList.push_back(mod);

	} else if ( ms.compare("on") == 0 ) {

		string armID ;
		for ( int k = 0 ; k < numParameterTags ; k++) {
			string parameterName = XML.getAttribute("parameter", "name", "PARAMETER_NAME", k );

			if ( parameterName.compare("armID") == 0 )	{
				armID = XML.getAttribute("parameter", "value", "0", k );

			} else {
				string tmp = "NOT WANTED ARM PARAMETER";
				tmp = tmp + ofUtil::ofToString(bs.m_startTime, 4);
				//ofLog::write(OF_LOG_ERROR, tmp.c_str());

				abort();
				return false;
			}
		}

		//ofLog::write(OF_LOG_VERBOSE, "on Arm ID %s ", armID.c_str() );
		Modality mod("arm", "on");

		mod.addParameter("armID", armID );

		bs.m_modalityList.push_back(mod);

	} else {
		//ofLog::write(OF_LOG_ERROR, "No function Matching Arm");
		//abort();
	}
	return true;
}

bool ExpressionDBLoader::engkeyArm2DOF( int numParameterTags, ExpressionSequence &bs ) {
	string armID, angle0, angle1, angle2 ;
	string speed;

	for ( int k = 0 ; k < numParameterTags ; k++) {

		string parameterName = XML.getAttribute("parameter", "name", "PARAMETER_NAME", k );
		if ( parameterName.compare("armID") == 0 ) {
			armID = XML.getAttribute("parameter", "value", "0", k );
		} else if ( parameterName.compare("angle[0]") == 0 ) {
			angle0 = XML.getAttribute("parameter", "value", "0", k );
		} else if ( parameterName.compare("angle[1]") == 0 ) {
			angle1 = XML.getAttribute("parameter", "value", "0", k );
		} else if ( parameterName.compare("speed") == 0 ) {
			speed = XML.getAttribute("parameter", "value", "10", k );
		} else {
			//ofLog::write(OF_LOG_ERROR, "NOT WANTED ARM PARAMETER");
			abort();
			return false;
		}
	}

	angle2 = angle0 ;
	angle0 = ofUtil::ofToString(100);
	angle1 = ofUtil::ofToString( ofUtil::ofToFloat(angle1)+90 );

	Modality mod("arm", "moveToPositionAll");

	mod.addParameter("armID", armID );
	mod.addParameter("angle[0]", angle0 );
	mod.addParameter("angle[1]", angle1 );
	mod.addParameter("angle[2]", angle2 );
	mod.addParameter("speed", speed );

	bs.m_modalityList.push_back(mod);

	return true;
}



bool ExpressionDBLoader::engkeyArm3DOF( int numParameterTags, ExpressionSequence &bs ) {
	string armID, angle0, angle1, angle2 ;
	string speed;

	for ( int k = 0 ; k < numParameterTags ; k++) {

		string parameterName = XML.getAttribute("parameter", "name", "PARAMETER_NAME", k );
		if ( parameterName.compare("armID") == 0 ) {
			armID = XML.getAttribute("parameter", "value", "0", k );
		} else if ( parameterName.compare("angle[0]") == 0 ) {
			angle0 = XML.getAttribute("parameter", "value", "0", k );
		} else if ( parameterName.compare("angle[1]") == 0 ) {
			angle1 = XML.getAttribute("parameter", "value", "0", k );
		} else if ( parameterName.compare("angle[2]") == 0 ) {
			angle2 = XML.getAttribute("parameter", "value", "0", k );
		} else if ( parameterName.compare("speed") == 0 ) {
			speed = XML.getAttribute("parameter", "value", "10.0", k );
		} else {
			//ofLog::write(OF_LOG_ERROR, "NOT WANTED ARM PARAMETER");
			abort();
			return false;
		}
	}
	Modality mod("arm", "moveToPositionAll");

	mod.addParameter("armID", armID );
	mod.addParameter("angle[0]", angle0 );
	mod.addParameter("angle[1]", angle1 );
	mod.addParameter("angle[2]", angle2 );
	mod.addParameter("speed", speed );

	bs.m_modalityList.push_back(mod);

	return true;
}

bool ExpressionDBLoader::eRobotArm3DOF( int numParameterTags, ExpressionSequence &bs ){
	string armID, angle0, angle1, angle2, speed0, speed1, speed2;
	for(int k = 0;k < numParameterTags;k++){
		string parameterName = XML.getAttribute("parameter", "name", "PARAMETER_NAME", k);
		if(parameterName.compare("armID") == 0){
			armID = XML.getAttribute("parameter", "value", "0", k);
		}else if(parameterName.compare("angle[0]") == 0){
			angle0 = XML.getAttribute("parameter", "value", "0", k);
		}else if(parameterName.compare("angle[1]") == 0){
			angle1 = XML.getAttribute("parameter", "value", "0", k);
		}else if(parameterName.compare("angle[2]") == 0){
			angle2 = XML.getAttribute("parameter", "value", "0", k);
		}else if(parameterName.compare("speed[0]") == 0){
			speed0 = XML.getAttribute("parameter", "value", "10.0", k);
		}else if(parameterName.compare("speed[1]") == 0){
			speed1 = XML.getAttribute("parameter", "value", "10.0", k);
		}else if(parameterName.compare("speed[2]") == 0){
			speed2 = XML.getAttribute("parameter", "value", "10.0", k);
		}else{
			//ofLog::write(OF_LOG_ERROR, "NOT WANTED ARM PARAMETER");
			abort();
			return false;
		}
	}

	Modality mod("arm", "moveToPositionWithAxisSpeed");
	mod.addParameter("armID", armID);
	mod.addParameter("angle[0]", angle0);
	mod.addParameter("angle[1]", angle1);
	mod.addParameter("angle[2]", angle2);
	mod.addParameter("speed[0]", speed0);
	mod.addParameter("speed[1]", speed1);
	mod.addParameter("speed[2]", speed2);
	bs.m_modalityList.push_back(mod);
	return true;
}

bool ExpressionDBLoader::getSoundModality( Modality & mod ) {

	bool res = false;
	for (int i = 0 ; i < expressionSequenceList.size() ; i++)	{
		int len = expressionSequenceList.at(i).m_modalityList.size();
		for ( int j = 0 ; j < len ; j++) {
			if ( expressionSequenceList.at(i).m_modalityList.at(j).ModalityName().compare("soundwav") == 0 ) {
				mod = expressionSequenceList.at(i).m_modalityList.at(j);
				return true;
			}
		}
	}
	return res;
}

int ExpressionDBLoader::findSpeakModality() {
	int count=0;
	for (int i = 0 ; i < expressionSequenceList.size() ; i++)	{
		int len = expressionSequenceList.at(i).m_modalityList.size();
		for ( int j = 0 ; j < len ; j++) {
			if ( expressionSequenceList.at(i).m_modalityList.at(j).ModalityName().compare("ttswav") == 0 ) {
				count++;
			}
		}
	}

	if ( count > 1 ) {
		removeSpeakModalities(count);
	}
	if ( count == 0 ) {
		Modality mod("ttswav", "playtts");
		expressionSequenceList.at(0).m_modalityList.push_back(mod);
	}
	return count;
}

void ExpressionDBLoader::removeSpeakModalities( int count ) {
	for (int i = (expressionSequenceList.size()-1) ; i >= 0 ; i--) {
		int len = expressionSequenceList.at(i).m_modalityList.size();
		for ( int j = (len-1) ; j >= 0 ; j--) {
			if ( expressionSequenceList.at(i).m_modalityList.at(j).ModalityName().compare("ttswav") == 0 ) {
				if ( count > 1)	{
					expressionSequenceList.at(i).m_modalityList.erase(expressionSequenceList.at(i).m_modalityList.begin()+j);
					count--;
				} else {
					break;
				}
			}
		}
	}
}


bool ExpressionDBLoader::loadEngkeyAvatarMpExpressEmotion( ExpressionSequence &bs, string emo ) {
	int numParameterTags = XML.getNumTags("parameter");
	string duration, weight;

	for ( int k = 0 ; k < numParameterTags ; k++) {
		string parameterName = XML.getAttribute("parameter", "name", "PARAMETER_NAME", k );
		if ( parameterName.compare("duration") == 0 ) {
			duration = XML.getAttribute("parameter", "value", "0", k );
		} else if ( parameterName.compare("weight") == 0 ) {
			weight = XML.getAttribute("parameter", "value", "0", k );
		}
	}
	//ofLog::write(OF_LOG_NOTICE, "duration = %s, weight = %s", duration.c_str(), weight.c_str() );
	Modality mod("avatar", emo);
	mod.addParameter("duration", duration);
	mod.addParameter("weight", weight);

	bs.m_modalityList.push_back(mod);

	return true;
}

bool ExpressionDBLoader::loadEngkeyAvatarLookAt( ExpressionSequence &bs )
{
	int numParameterTags = XML.getNumTags("parameter");

	string duration, weight, x,y;

	for ( int k = 0 ; k < numParameterTags ; k++) {

		string parameterName = XML.getAttribute("parameter", "name", "PARAMETER_NAME", k );
		if ( parameterName.compare("duration") == 0 ) {
			duration = XML.getAttribute("parameter", "value", "0", k );
		} else if ( parameterName.compare("weight") == 0 ) {
			weight = XML.getAttribute("parameter", "value", "0", k );
		} else if ( parameterName.compare("x") == 0 ) {
			x = XML.getAttribute("parameter", "value", "0", k );
		} else if ( parameterName.compare("y") == 0 ) {
			y = XML.getAttribute("parameter", "value", "0", k );
		}
	}
	//ofLog::write(OF_LOG_NOTICE, "duration = %s, weight = %s", duration.c_str(), weight.c_str() );

	Modality mod("avatar", "mpLookAt");
	mod.addParameter("duration", duration);
	mod.addParameter("x", x);
	mod.addParameter("y", y);
	mod.addParameter("weight", weight);

	bs.m_modalityList.push_back(mod);

	return true;
}

bool ExpressionDBLoader::loadEngkeyAvatarCloseEye( ExpressionSequence &bs ) {
	int numParameterTags = XML.getNumTags("parameter");
	string duration, close;
	for ( int k = 0 ; k < numParameterTags ; k++) {

		string parameterName = XML.getAttribute("parameter", "name", "PARAMETER_NAME", k );
		if ( parameterName.compare("duration") == 0 ) {
			duration = XML.getAttribute("parameter", "value", "0", k );
		} else if ( parameterName.compare("close") == 0 ) {
			close = XML.getAttribute("parameter", "value", "0", k );
		}
	}
	// ofLog::write(OF_LOG_NOTICE, "duration = %s, close = %s", duration.c_str(), close.c_str() );

	Modality mod("avatar", "mpCloseEye");
	mod.addParameter("duration", duration);
	mod.addParameter("close", close);

	bs.m_modalityList.push_back(mod);

	return true;

}
//mpBlink
bool ExpressionDBLoader::loadEngkeyAvatarBlink( ExpressionSequence &bs ) {
	int numParameterTags = XML.getNumTags("parameter");
	string type, gain;
	for ( int k = 0 ; k < numParameterTags ; k++) {

		string parameterName = XML.getAttribute("parameter", "name", "PARAMETER_NAME", k );
		if ( parameterName.compare("type") == 0 ) {
			type = XML.getAttribute("parameter", "value", "0", k );
		} else if ( parameterName.compare("gain") == 0 ) {
			gain = XML.getAttribute("parameter", "value", "0", k );
		}
	}
	// ofLog::write(OF_LOG_NOTICE, "duration = %s, close = %s", type.c_str(), gain.c_str() );

	Modality mod("avatar", "mpBlink");
	mod.addParameter("type", type);
	mod.addParameter("gain", gain);

	bs.m_modalityList.push_back(mod);

	return true;
}

bool ExpressionDBLoader::loadEngkeyAvatarPupilPos( ExpressionSequence &bs ) {
	int numParameterTags = XML.getNumTags("parameter");
	string side, x, y;

	for ( int k = 0 ; k < numParameterTags ; k++) {

		string parameterName = XML.getAttribute("parameter", "name", "PARAMETER_NAME", k );
		if ( parameterName.compare("side") == 0 ) {
			side = XML.getAttribute("parameter", "value", "0", k );
		} else if ( parameterName.compare("x") == 0 ) {
			x = XML.getAttribute("parameter", "value", "0", k );
		} else if ( parameterName.compare("y") == 0 ) {
			y = XML.getAttribute("parameter", "value", "0", k );
		}
	}
	Modality mod("avatar", "mpSetPupilPos");
	mod.addParameter("side", side);
	mod.addParameter("x", x);
	mod.addParameter("y", y);

	bs.m_modalityList.push_back(mod);

	return true;
}

bool ExpressionDBLoader::loadEngkeyAvatarSetEyeClose( ExpressionSequence &bs )
{
	int numParameterTags = XML.getNumTags("parameter");

	string side, close;
	for ( int k = 0 ; k < numParameterTags ; k++) {

		string parameterName = XML.getAttribute("parameter", "name", "PARAMETER_NAME", k );
		if ( parameterName.compare("side") == 0 ) {
			side = XML.getAttribute("parameter", "value", "0", k );
		} else if ( parameterName.compare("close") == 0 ) {
			close = XML.getAttribute("parameter", "value", "0", k );
		}
	}

	// ofLog::write(OF_LOG_NOTICE, "duration = %s, close = %s", side.c_str(), close.c_str() );

	Modality mod("avatar", "mpSetEyeClose");
	mod.addParameter("side", side);
	mod.addParameter("close", close);

	bs.m_modalityList.push_back(mod);

	return true;
}

bool ExpressionDBLoader::loadAvatarFaceParam( ExpressionSequence &bs )
{
	int numParameterTags = XML.getNumTags("parameter");

	string face, duration;
	for ( int k = 0 ; k < numParameterTags ; k++) {

		string parameterName = XML.getAttribute("parameter", "name", "PARAMETER_NAME", k );
		if ( parameterName.compare("face") == 0 ) {
			face = XML.getAttribute("parameter", "value", "0", k );
		} else if ( parameterName.compare("duration") == 0 ) {
			duration = XML.getAttribute("parameter", "value", "0", k );
		}
	}

	// ofLog::write(OF_LOG_NOTICE, "face = %s, duration = %s", face.c_str(), duration.c_str() );

	Modality mod("avatar", "faceParam");
	mod.addParameter("face", face);
	mod.addParameter("duration", duration);

	bs.m_modalityList.push_back(mod);

	return true;
}

bool ExpressionDBLoader::loadAvatarPlayMocap( ExpressionSequence &bs )
{
	int numParameterTags = XML.getNumTags("parameter");

	string mocapDir, mocapFile, songDir, songFile;
	for ( int k = 0 ; k < numParameterTags ; k++) {

		string parameterName = XML.getAttribute("parameter", "name", "PARAMETER_NAME", k );
		if ( parameterName.compare("mocapDir") == 0 ) {
			mocapDir = XML.getAttribute("parameter", "value", "0", k );
		} else if ( parameterName.compare("mocapFile") == 0 ) {
			mocapFile = XML.getAttribute("parameter", "value", "0", k );
		} else if ( parameterName.compare("songDir") == 0 ) {
			songDir = XML.getAttribute("parameter", "value", "0", k );
		} else if ( parameterName.compare("songFile") == 0 ) {
			songFile = XML.getAttribute("parameter", "value", "0", k );
		}
	}

//	ofLog::write(OF_LOG_NOTICE, "mocapDir = %s, mocapFile = %s, songDir = %s, songFile", mocapDir.c_str(), mocapFile.c_str(), songDir.c_str(), songFile.c_str() );

	Modality mod("avatar", "playMocap");
	mod.addParameter("mocapDir", mocapDir);
	mod.addParameter("mocapFile", mocapFile);
	mod.addParameter("songDir", songDir);
	mod.addParameter("songFile", songFile);

	bs.m_modalityList.push_back(mod);

	return true;
}

std::string ExpressionDBLoader::makeFilenameFromExpressionInfo( const ST_Expression& exp, bool isXML) {

	string result =  "/opt/robocare/silbot3/expression/expression_"+exp.SentenceType()+"_"+exp.EmotionID()+"_"+exp.ExpressionLength()+".xml";
	ROS_INFO("load Expression file : %s", result.c_str());
	return result;
}

bool  ExpressionDBLoader::loadPosition(ExpressionSequence &bs){

	int numParameterTags = XML.getNumTags("parameter");
	string x, y, t,speed;
	for ( int k = 0 ; k < numParameterTags ; k++) {
		string parameterName = XML.getAttribute("parameter", "name", "PARAMETER_NAME", k );
		if ( parameterName.compare("x") == 0 )	{
			x = XML.getAttribute("parameter", "value", "0", k );
		} else if ( parameterName.compare("y") == 0 ) {
			y = XML.getAttribute("parameter", "value", "0", k );
		} else if ( parameterName.compare("t") == 0 ) {
			t = XML.getAttribute("parameter", "value", "0", k );
		} else if ( parameterName.compare("speed") == 0 ) {
			speed = XML.getAttribute("parameter", "value", "0", k );
		} else {
			return false;
		}
	}

	//ofLog::write(OF_LOG_VERBOSE, "degreeUpDown = %s, degreeLeftRight = %s, speed = %s", str_degreeRoll.c_str(), str_degreeBend.c_str(), str_speed.c_str());

	Modality mod("position", "setPosition");

	mod.addParameter("x", x);
	mod.addParameter("y", y);
	mod.addParameter("t", t);
	mod.addParameter("speed", speed);
	bs.m_modalityList.push_back(mod);
	return true;
}

void ExpressionDBLoader::setSpeakTime()
{
	ExpressionSequence bs;
	double time;
}

} /* namespace db */
} /* namespace Expression */
