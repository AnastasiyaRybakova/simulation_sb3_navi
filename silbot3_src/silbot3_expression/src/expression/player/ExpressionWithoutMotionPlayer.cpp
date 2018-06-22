#include "ExpressionWithoutMotionPlayer.h"

namespace expression {
namespace player {

ExpressionWithoutMotionPlayer::ExpressionWithoutMotionPlayer() {
	this->speakNow = false;

	this->lipsync = Lipsync::getInstance();

	this->statusNotificationListener = NULL;
}

ExpressionWithoutMotionPlayer::~ExpressionWithoutMotionPlayer() {
}

void ExpressionWithoutMotionPlayer::startPlay() throw (CannotPlayException) {
	ExpressionDBLoader dbLoader;
//	string filename = ExpressionDBLoader::makeFilenameFromExpressionInfo(this->parameter.m_behaviorType);
//	bool result = dbLoader.loadExpressionDBFromFile(filename, this->db);
	bool result = true;
	if(result) {
		ROS_INFO("ExpressionWithSentencePlayer : DB Load Succeeded");
		this->speakThread = boost::thread(boost::bind(&ExpressionWithoutMotionPlayer::playLipsyncThread, this));

	} else {
		CannotPlayException e;
		e.setMessage("DB File error");
		e.setMark(__FILE__, __LINE__);
		throw e;
	}
}

void ExpressionWithoutMotionPlayer::playLipsyncThread() {

	this->speakNow = true;

	try {
		string sentence = parameter.Sentence();
		this->lipsync->lipSync(sentence);
	} catch(expression::exception::IOException& e) {
		e.setMark(__FILE__, __LINE__);
		e.printStackTrace();
	}
	this->speakNow = false;
	this->statusNotificationListener->handleNotification(this->parameter.UUID(), expression::notification::SOUND_FINISH, expression::status::EXPRESSION_STATUS_WITHOUT_MOTION);
}

void ExpressionWithoutMotionPlayer::stopPlay(){
	this->lipsync->stop();
	while(this->speakNow) {
		ros::Duration(0.2).sleep();
		ROS_INFO("%s", "Wating For ExpressionWithSentence finishing");
	}

	this->speakNow = false;
}

void ExpressionWithoutMotionPlayer::setParameter(const ExpressionParameter& parameter) {
	this->parameter = parameter;
}

void ExpressionWithoutMotionPlayer::setModalityUsetSet(const ModalityUseSet& modalityUsetSet) {
	this->modality = modalityUsetSet;
}


void ExpressionWithoutMotionPlayer::setstatusNotificationListener(StatusNotificationListener* listener) {
	this->statusNotificationListener = listener;
}


} /* namespace player */
} /* namespace behavior */
