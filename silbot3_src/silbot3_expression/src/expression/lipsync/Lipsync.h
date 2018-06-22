#ifndef _EXPRESSION_LIPSYNC_LIPSYNC_H_
#define _EXPRESSION_LIPSYNC_LIPSYNC_H_


#include <string>
#include <vector>
#include <ros/ros.h>
#include <silbot3_msgs/TTSViseme.h>
#include <expression/exception/IOException.h>
#include <expression/connector/AvatarConnector.h>
#include <expression/connector/SoundConnector.h>

using namespace std;
using namespace expression::connector;

namespace expression{
namespace lipsync {

class SentenceUnit {
public:
	bool isRecording;
	string sentence;

	SentenceUnit() {
		this->isRecording = false;
		this->sentence = "";
	}
	virtual ~SentenceUnit(){}
};

class Lipsync {
private:

	SoundConnector* soundConn;
	AvatarConnector* avatarConn;
	bool m_islipsyncstop;

	void playSound(bool blocking, const string& filename);
	void split(string str, vector<SentenceUnit>& result);
public:

	const static int __DEFAULT_VOL;
	const static int __DEFAULT_SPEED;

	Lipsync();
	virtual ~Lipsync();

	static Lipsync* getInstance();

	bool lipSync(string sentence);
	bool songPlay(string filename);
	bool stop();
	bool getSentenceInfo(const string& sentence, string& filename, string& dirpath, vector<silbot3_msgs::TTSViseme>& visemes) throw (expression::exception::IOException);

};

//vector<string> split( const string& s, const string& f );

} /* namespace expression */
}
#endif /* _EXPRESSION_LIPSYNC_LIPSYNC_H_ */
