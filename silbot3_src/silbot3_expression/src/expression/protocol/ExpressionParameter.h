#ifndef __EXPRESSION_PARAMETER_H__
#define __EXPRESSION_PARAMETER_H__

#include <string>
#include <vector>
#include <sstream>

#include <boost/lexical_cast.hpp>

using namespace std;

namespace expression {
namespace parameter {

namespace LanguageSpeakerID {
	enum SpeakerList {
		KOR_MAN_01 = 0,
		KOR_MAN_02,
		KOR_MAN_03,
		KOR_MAN_04,
		KOR_MAN_05,
		KOR_WOMAN_01 = 5,
		KOR_WOMAN_02 ,
		KOR_WOMAN_03 ,
		KOR_WOMAN_04 ,
		KOR_WOMAN_05 ,

		ENG_MAN_01 = 10,
		ENG_MAN_02,
		ENG_MAN_03,
		ENG_MAN_04,
		ENG_MAN_05,
		ENG_WOMAN_01 = 15,
		ENG_WOMAN_02 ,
		ENG_WOMAN_03 ,
		ENG_WOMAN_04 ,
		ENG_WOMAN_05 ,

		DEN_MAN_01 = 20,
		DEN_MAN_02,
		DEN_MAN_03,
		DEN_MAN_04,
		DEN_MAN_05,
		DEN_WOMAN_01 = 25,
		DEN_WOMAN_02 ,
		DEN_WOMAN_03 ,
		DEN_WOMAN_04 ,
		DEN_WOMAN_05 ,

		FIN_MAN_01 = 30,
		FIN_MAN_02,
		FIN_MAN_03,
		FIN_MAN_04,
		FIN_MAN_05,
		FIN_WOMAN_01 = 35,
		FIN_WOMAN_02 ,
		FIN_WOMAN_03 ,
		FIN_WOMAN_04 ,
		FIN_WOMAN_05 ,
		NOT_DEFINED = 100
	};

}

class ST_Expression{
public:
	string m_sentenceType;
	string m_emotionID;
	string m_expressionLength;

	static const int LongLength = 20;

public:
	ST_Expression(string sentenceType, string emotionID, string expressionLength);
	ST_Expression(void);
	~ST_Expression(void);


	std::string SentenceType() const { return m_sentenceType; }
	void SentenceType(std::string val) { m_sentenceType = val; }

	std::string EmotionID() const { return m_emotionID; }
	void EmotionID(std::string val) { m_emotionID = val; }

	std::string ExpressionLength() const { return m_expressionLength; }
	void ExpressionLength(std::string val) { m_expressionLength = val; }

	void checkAndSetExpressionLength( string sentence );


};

class ExpressionParameter {
public:
	ExpressionParameter();
	virtual ~ExpressionParameter();

	string m_messagename;
	double m_startTime;
	double m_playttsTime;
	int m_endTime;
	double m_panoffset;
	double m_tiltoffset;
	bool m_randomMode;
	string m_sentence;
	ST_Expression m_behaviorType;
	LanguageSpeakerID::SpeakerList m_speakerID;
	string m_recordedVoice;
	int m_recordedSentenceID;
	int RecordedSentenceID() const { return m_recordedSentenceID; }
	// 0 : pre 1:post
	int m_wavPosition;
	int m_moveToPanHome;
	int m_TTSSpeed;
	int TTSSpeed() const { return m_TTSSpeed; }
	int m_DefaultTTSSpeed;
	int m_TTSVolume;
	int m_DefaultTTSVolume;
	vector<string> kiboInfo;
	string m_filename;
	string m_uuid;
	string m_songpath;

public:
	void fromParameter( string package,
						string category,
						string id,
						string content);

	std::string UUID() const { return m_uuid; }
	void UUID(std::string val) { m_uuid = val; }
	std::string Messagename() const { return m_messagename; }
	void Messagename(std::string val) { m_messagename = val; }
	double StartTime() const { return m_startTime; }
	void StartTime(double val) { m_startTime = val; }
	double PlayttsTime() const { return m_playttsTime; }
	void PlayttsTime(double val) { m_playttsTime = val; }
	int EndTime() const { return m_endTime; }
	void EndTime(int val) { m_endTime = val; }
	int Panoffset() const { return m_panoffset; }
	void Panoffset(int val) { m_panoffset = val; }
	int Tiltoffset() const { return m_tiltoffset; }
	void Tiltoffset(int val) { m_tiltoffset = val; }
	bool RandomMode() const { return m_randomMode; }
	void RandomMode(bool val) { m_randomMode = val; }
	std::string Sentence() const { return m_sentence; }
	void Sentence(std::string val) { m_sentence = val; }
	ST_Expression Behavior() const { return m_behaviorType; }
	void Behavior(ST_Expression val) { m_behaviorType = val; }
	LanguageSpeakerID::SpeakerList SpeakerID() const { return m_speakerID; }
	void SpeakerID(LanguageSpeakerID::SpeakerList val) { m_speakerID = val; }
	std::string RecordedVoiceFilename() const { return m_recordedVoice; }
	void setRecordedVoiceFilename(const std::string& val) { m_recordedVoice = val; }
	void RecordedSentenceID(int val) { m_recordedSentenceID = val; }
	int WavPosition() const { return m_wavPosition; }
	void WavPosition(int val) { m_wavPosition = val; }
	int MoveToPanHome() const { return m_moveToPanHome; }
	void MoveToPanHome(int val) { m_moveToPanHome = val; }
	void TTSSpeed(int val) { m_TTSSpeed = val; }
	int DefaultTTSSpeed() const { return m_DefaultTTSSpeed; }
	void DefaultTTSSpeed(int val) { m_DefaultTTSSpeed = val; }
	int TTSVolume() const { return m_TTSVolume; }
	void TTSVolume(int val) { m_TTSVolume = val; }
	int DefaultTTSVolume() const { return m_DefaultTTSVolume; }
	void DefaultTTSVolume(int val) { m_DefaultTTSVolume = val; }


	const string& getSongPath() const {
		return m_songpath;
	}

	string toString();

	const string& getDBFileName() const;

};

} /* namespace parameter */
} /* namespace expression */

#endif /* __BEHAVIOR_PROTOCOL_BEHAVIOREXPRESSIONPARAMETER_H__ */
