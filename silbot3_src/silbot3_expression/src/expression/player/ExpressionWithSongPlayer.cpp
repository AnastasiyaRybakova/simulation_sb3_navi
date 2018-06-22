#include "ExpressionWithSongPlayer.h"
#include <boost/tokenizer.hpp>
#include <vector>
using namespace std;

using namespace expression::player;

ExpressionWithSongPlayer::ExpressionWithSongPlayer() {
	this->expressNow = false;
	this->songNow = false;

	lipsync = Lipsync::getInstance();
	_songName = "";
	_songDIr = "";

	this->statusNotificationListener = NULL;
}

ExpressionWithSongPlayer::~ExpressionWithSongPlayer() {

}

void ExpressionWithSongPlayer::startPlay() throw (CannotPlayException) {

	string filename = ExpressionDBLoader::makeFilenameFromExpressionInfo(this->parameter.m_behaviorType);
	//cout << "filename : " << filename << endl;
	this->statusNotificationListener->handleNotification(this->parameter.UUID(), expression::notification::START, expression::status::EXPRESSION_STATUS_WITH_SONG);
	bool result = dbLoader.loadExpressionDBFromFile(filename, this->db);
	_songName = dbLoader.getSongName();
	_songDIr = dbLoader.getSongDir();

	ROS_INFO("_songDIr: %s  _songName :  %s  ",_songDIr.c_str(), _songName.c_str());

	if(result) {
		ROS_INFO("ExpressionWithSongPlayer : DB Load Succeeded");

		this->songThread = boost::thread(boost::bind(&ExpressionWithSongPlayer::playSongThread, this));
		this->behaviorThread = boost::thread(boost::bind(&ExpressionWithSongPlayer::playExpressionThread, this));
	} else {
		CannotPlayException e;
		e.setMessage("DB File error");
		e.setMark(__FILE__, __LINE__);
		throw e;
	}
}

void ExpressionWithSongPlayer::stopPlay() {
	expression.setExressionFlag(false);

	this->lipsync->stop();

	while(this->expressNow || this->songNow ) {
		ros::Duration(0.2).sleep();
		ROS_INFO("%s", "Wating For SongPlay finishing");
	}

	this->expressNow = false;
	this->songNow = false;
}

void ExpressionWithSongPlayer::playExpressionThread() {

	this->expressNow = true;
	expression.setExressionFlag(true);
//	expression.moveToHome(this->parameter, this->modality);
	expression.executeDB(this->parameter, this->modality, this->db);
	expression.setExressionFlag(false);
	this->expressNow = false;

	this->statusNotificationListener->handleNotification(this->parameter.UUID(), expression::notification::MOTION_FINISH, expression::status::EXPRESSION_STATUS_WITH_SONG);
}

void ExpressionWithSongPlayer::playSongThread() {
	this->songNow = true;

	try {
		string songpath = this->parameter.getSongPath();

//		double startTime = ros::Time::now().toSec();
//		const std::vector<BehaviorSequence>& behaviorSequenceList  = db.getBehaviorSequenceList();
//
//		double sleepTime = (bs.m_startTime - ellapsedTime);
		double sleepTime = this->dbLoader.getSongTime();
		if(sleepTime >= 0.01) {
			ros::Duration(sleepTime).sleep();
		}
		ROS_INFO("songPlay : %s",this->_songName.c_str());
		this->lipsync->songPlay(this->_songName);


	} catch(expression::exception::IOException& e) {
		e.setMark(__FILE__, __LINE__);
		e.printStackTrace();
	}
	this->songNow = false;
	this->statusNotificationListener->handleNotification(this->parameter.UUID(), expression::notification::SOUND_FINISH, expression::status::EXPRESSION_STATUS_WITH_SONG);
}

void ExpressionWithSongPlayer::setParameter(const ExpressionParameter& param) {
	this->parameter = param;
}

void ExpressionWithSongPlayer::setModalityUsetSet(const ModalityUseSet& modalityUsetSet) {
	this->modality = modalityUsetSet;
}


void ExpressionWithSongPlayer::setstatusNotificationListener(StatusNotificationListener* listener) {
	this->statusNotificationListener = listener;
}
