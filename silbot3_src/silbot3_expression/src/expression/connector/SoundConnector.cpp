#include "SoundConnector.h"
#include <expression/exception/Exception.h>
#include <boost/lexical_cast.hpp>
#include <ros/service.h>

using namespace expression::connector;
using namespace expression::lipsync;

const int VisemeInfo::__VISEMETYPE_SENTENCE = 0;
const int VisemeInfo::__VISEMETYPE_RECORD = 1;

SoundConnector::SoundConnector() {
	this->makeTTSWavServiceClient = nh.serviceClient<silbot3_msgs::TTSMake>("/silbot3_tts/make", false);
	this->playServiceClient = nh.serviceClient<silbot3_msgs::SoundPlay>("/silbot3_sound/play", false);
	this->playPublisher = nh.advertise<std_msgs::String>("/silbot3_sound/play", 1000);
	this->stopPublisher = nh.advertise<std_msgs::Empty>("/silbot3_sound/stop", 1000);
	visemeInfoParser.parse();
	/*this->ttsplayClient = nh.serviceClient<silbot3_msgs::ttsplaysrv>("/tts/playsrv")*/
}

SoundConnector::~SoundConnector() {
}

SoundConnector* SoundConnector::getInstance() {
	static SoundConnector _thisInstance;
	return &_thisInstance;
}


void SoundConnector::getRecordingVisem(const int& rid, string& filename, vector<silbot3_msgs::TTSViseme>& visemes) throw (expression::exception::IOException) {

	// int recordID;
	// string text;
	// string visemes;
	// string wavfilename;
	// vector<Viseme> visemeInfos;
	RecordedVisemeInfo info = visemeInfoParser.getViseme(rid);
	filename = info.wavfilename;
	vector<expression::db::Viseme> infos = info.visemeInfos;
	for(int i=0; i<infos.size() ; i++) {
		silbot3_msgs::TTSViseme vi;
		vi.id = infos[i].visemeID;
		vi.duration = infos[i].duration;
		visemes.push_back(vi);
		// cout << "fffffffffffffff" << endl;
	}
}

void SoundConnector::makeTTSWav(const string& sentence, string& filename, vector<silbot3_msgs::TTSViseme>& visemes) throw (expression::exception::IOException) {
	silbot3_msgs::TTSMake srv;
	srv.request.text = sentence;
	srv.request.filepath = filename;

	ROS_INFO("wating for service will be available........../silbot3_tts/make");
	bool success = ros::service::waitForService("/silbot3_tts/make");
	if(!success) {
		ROS_ERROR("/silbot3_tts/make service is not available yet!!");
	} else {
		ROS_INFO("/silbot3_tts/make service available..");
	}

	if(makeTTSWavServiceClient.call(srv)) {

		for(int i = 0; i < srv.response.tts_viseme_array.size(); i++) {
			visemes.push_back(srv.response.tts_viseme_array.at(i));
		}
	}
	else {
		expression::exception::IOException e;
		e.setMark(__FILE__, __LINE__);
		e.setMessage("cannot call /silbot3_tts/make");
		throw e;
	}

}

void SoundConnector::stop()  throw (expression::exception::IOException) {
	std_msgs::Empty msg;
	stopPublisher.publish(msg);
}

void SoundConnector::playSound( bool blocking, const string& filepath ) throw (expression::exception::IOException)
{
	try {

		if(blocking) {
			silbot3_msgs::SoundPlay playSrv;
			playSrv.request.filepath = filepath;

			ROS_INFO("SoundConnector::playSoundbool : wating for service will be available..........playsound_sync");
			bool success = ros::service::waitForService("/silbot3_sound/play");
			if(!success) {
				ROS_ERROR("SoundConnector::playSoundbool : playsound_sync service is not available yet!!");
			} else {
				ROS_INFO("SoundConnector::playSoundbool : playsound_sync service available..");
			}

			if(playServiceClient.call(playSrv)) {
				 int result = playSrv.response.sound_result;
				 ROS_INFO("Voice Play OK !!");
			} else {
				int result = playSrv.response.sound_result;
				expression::exception::IOException e;
				e.setMark(__FILE__, __LINE__);
				e.setMessage("-ERROR : cannot call service /silbot3_sound/play : "+result);
			}

		} else {
			std_msgs::String msg;
			msg.data = filepath;

			playPublisher.publish(msg);
		}

	} catch(expression::exception::IOException& e) {
		e.setMark(__FILE__, __LINE__);
		throw e;
	}
}
