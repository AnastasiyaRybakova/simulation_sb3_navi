#include "ExpressionWithMotionPlayer.h"

namespace expression {
namespace player {


ExpressionWithMotionPlayer::ExpressionWithMotionPlayer() {
	this->expressNow = false;
	this->speakNow = false;

	lipsync = Lipsync::getInstance();
	//
	// will be setstatusNotificationListener
	//
	//
	this->statusNotificationListener = NULL;
}

ExpressionWithMotionPlayer::~ExpressionWithMotionPlayer() {
}

void ExpressionWithMotionPlayer::startPlay() throw (CannotPlayException) {
	ExpressionDBLoader dbLoader;
	string filename = ExpressionDBLoader::makeFilenameFromExpressionInfo(this->parameter.m_behaviorType);

	//cout << "filename : " << filename << endl;
	ROS_DEBUG("ExpressionWithMotionPlayer : DB File Name: %s", filename.c_str());

	bool result = dbLoader.loadExpressionDBFromFile(filename, this->db);

	if(result) {
		ROS_INFO("ExpressionWithMotionPlayer : DB Load Succeeded %s", filename.c_str());
		this->statusNotificationListener->handleNotification(this->parameter.UUID(), expression::notification::START, expression::status::EXPRESSION_STATUS_WITH_MOTION);
		//
		// lipsync thread start
		//
		//
		this->speakThread = boost::thread(boost::bind(&ExpressionWithMotionPlayer::playLipsyncThread, this));

		//
		// behavior player thread start
		//
		//
		this->behaviorPlayThread = boost::thread(boost::bind(&ExpressionWithMotionPlayer::playExpressionThread, this));

	} else {
		CannotPlayException e;
		e.setMessage("DB File error");
		e.setMark(__FILE__, __LINE__);
		throw e;
	}
}

void ExpressionWithMotionPlayer::playExpressionThread() {
	this->expressNow = true;

	expression.setExressionFlag(true);
//	expression.moveToHome(this->parameter, this->modality);
	expression.executeDB(this->parameter, this->modality, this->db);
	expression.setExressionFlag(false);

	this->expressNow = false;

	this->statusNotificationListener->handleNotification(this->parameter.UUID(), expression::notification::MOTION_FINISH, expression::status::EXPRESSION_STATUS_WITH_MOTION);
}

void ExpressionWithMotionPlayer::playLipsyncThread() {

	this->speakNow = true;

	try {
		string sentence = parameter.Sentence();
		this->lipsync->lipSync(sentence);
		/*SentenceTokenizer tokenizer;
		vector<behavior::util::Token> result;
		tokenizer.tokenize(sentence, result);
		for(unsigned int i = 0; i < result.size(); i++) {
			behavior::util::Token tok = result[i];
			if(tok.type == behavior::util::TOKEN_TYPE_SENTENCE) {
				conn.speakSync(0, tok.sentence, 0);
			} else {
				conn.speakSync(tok.recordid, "", 0);
			}
		}*/
	} catch(expression::exception::IOException& e) {
		e.setMark(__FILE__, __LINE__);
		e.printStackTrace();
	}
	this->speakNow = false;
	this->statusNotificationListener->handleNotification(this->parameter.UUID(), expression::notification::SOUND_FINISH, expression::status::EXPRESSION_STATUS_WITH_MOTION);
}

void ExpressionWithMotionPlayer::stopPlay(){
	expression.setExressionFlag(false);
	this->lipsync->stop();
	while(this->expressNow || this->speakNow ) {
		ros::Duration(0.2).sleep();
		ROS_INFO("%s", "Wating For ExpressionWithSentence finishing");
//		cout << "expressNow : " << this->expressNow << endl;
//		cout << "speakNow : " << this->speakNow << endl;

	}

	this->expressNow = false;
	this->speakNow = false;
	this->kiboExpressNow = false;
}

void ExpressionWithMotionPlayer::setParameter(const ExpressionParameter& parameter) {
	this->parameter = parameter;
}

void ExpressionWithMotionPlayer::setModalityUsetSet(const ModalityUseSet& modalityUsetSet) {
	this->modality = modalityUsetSet;
}


void ExpressionWithMotionPlayer::setstatusNotificationListener(StatusNotificationListener* listener) {
	this->statusNotificationListener = listener;
}

//void ExpressionWithMotionPlayer::setParameter(const ExpressionParameter& parameter) {
//}


} /* namespace player */
} /* namespace expression */
