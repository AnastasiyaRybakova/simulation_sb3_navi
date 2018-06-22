#include "ExpressionParameter.h"

namespace expression {
namespace parameter {

ST_Expression::ST_Expression()
	: m_emotionID(""), m_sentenceType(""), m_expressionLength("short") {
}

ST_Expression::ST_Expression( string sentenceType, string emotionID, string expressionLength )
	: m_sentenceType(sentenceType), m_emotionID(emotionID), m_expressionLength(expressionLength) {

}

ST_Expression::~ST_Expression() {
}

void ST_Expression::checkAndSetExpressionLength( string sentence ) {
	int len = sentence.length();
	if ( ST_Expression::LongLength * 2 < len ) {
		m_expressionLength = "long";
		//ofLog::write(OF_LOG_WARNING, "Sentence Length Long");
	} else {
		m_expressionLength = "short";
		//ofLog::write(OF_LOG_WARNING, "Sentence Length Short");
	}
}


ExpressionParameter::ExpressionParameter() {
	kiboInfo.clear();
	m_panoffset = 0;
	m_tiltoffset = 0;
	m_sentence = "";
	m_randomMode= false;
	m_recordedVoice = "";
	m_startTime = -1;
	m_endTime = -1;
	m_messagename = "";
	m_moveToPanHome = 1;
	m_TTSSpeed = 100;
	m_DefaultTTSSpeed=100;
	m_TTSVolume=100;
	m_DefaultTTSVolume = 75;
	m_playttsTime = -1;
}

ExpressionParameter::~ExpressionParameter() {
}

void ExpressionParameter::fromParameter( string package, string category, string id, string content) {

	m_messagename = "ExpressionWithMotion";

	m_uuid = "default_uuid";

	m_behaviorType.SentenceType(boost::lexical_cast<std::string>(package));
	m_behaviorType.EmotionID(boost::lexical_cast<std::string>(category));
	m_behaviorType.ExpressionLength(boost::lexical_cast<std::string>(id));

	try {
		m_sentence		= boost::lexical_cast<std::string>(content);
	} catch(boost::bad_lexical_cast e) {
		m_sentence		= "";
	}

	this->m_songpath = content;
	m_startTime = -1;
	m_endTime = -1;
	m_moveToPanHome = 1;
	m_recordedVoice = "";
	m_TTSSpeed = DefaultTTSSpeed();
	m_TTSVolume  = DefaultTTSVolume();
	m_playttsTime = -1;
}

const string& ExpressionParameter::getDBFileName() const {
	return m_filename;
}

std::string ExpressionParameter::toString() {
	std::stringstream ss;
	ss<<"==============Behavior Expression Parameter Dump  ========== "<<endl;
	ss<<"\tpackage : ["<<m_behaviorType.m_sentenceType<<"]"<<endl;
	ss<<"\tcategory : ["<<m_behaviorType.m_emotionID<<"]"<<endl;
	ss<<"\tid : ["<<m_behaviorType.m_expressionLength<<"]"<<endl;
	ss<<"\tcontent : ["<<m_sentence<<"]"<<endl;
	ss<<"=============================="<<endl;
	return ss.str();
}

} /* namespace parameter */
}

 /* namespace expression */
