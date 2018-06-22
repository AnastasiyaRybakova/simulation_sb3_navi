#include "Lipsync.h"
#include <ros/package.h>
#include <boost/tokenizer.hpp>
//#include <boost/algorithm/string.hpp>

#include <expression/exception/ConnectException.h>
#include <expression/exception/NoSuchElementException.h>
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>
#include <boost/filesystem.hpp>
//#include <boost/thread.hpp>
//#include <robocare/env/CConfig.h>
//#include <robocare/lang/Integer.h>
//#include "FaceConnectionFactory.h"

using namespace expression::exception;
using namespace expression::lipsync;

const int Lipsync::__DEFAULT_VOL = 75;
const int Lipsync::__DEFAULT_SPEED = 100;

Lipsync::Lipsync() {
	//m_plipsyncInformation = new CLipsyncInformation;

	avatarConn = AvatarConnector::getInstance();
	soundConn = SoundConnector::getInstance();

	//avatarCon = new avatar::MPAvatarConnector();
	m_islipsyncstop = false;
}

Lipsync::~Lipsync() {

}

Lipsync* Lipsync::getInstance() {
	static Lipsync _thisInstance;
	return &_thisInstance;
}

bool Lipsync::stop() {
	ROS_INFO("Lipsync stop called");

	try {
		m_islipsyncstop = true;
		this->soundConn->stop();
	} catch(expression::exception::IOException& e) {
		e.setMark(__FILE__, __LINE__);
		e.printStackTrace();
	}

	return true;
}


bool Lipsync::lipSync(string sentence) {

	ROS_INFO("Lipsync callback executed");

	vector<silbot3_msgs::TTSViseme> visemes;
	string dirpath;
	// init stop flag for previous
	m_islipsyncstop = false;
	silbot3_msgs::TTSViseme zerov;
	zerov.id = 0;
	zerov.duration = 0.1;
	try {

		if((int)sentence.find("+") > -1){
			vector<SentenceUnit> words;
			split(sentence, words);
			for(int i = 0; i < words.size(); i++) {
				try {
					if(m_islipsyncstop){
						m_islipsyncstop = false;
						break;
					}
					else if(words[i].isRecording) {
						string filename;
						soundConn->getRecordingVisem(lexical_cast<int>(words[i].sentence), filename, visemes);
						avatarConn->playLipsync("tts", "", filename,visemes );
						playSound(true, filename);
					} else {
						boost::uuids::uuid uuid = boost::uuids::random_generator()();
						stringstream filestream;
						filestream << "/tmp/" << uuid << ".wav";
						string filename = filestream.str();
						getSentenceInfo(words[i].sentence, filename, dirpath, visemes);
						// this->faceCon->playLipsync(visemes);
						//visemes.push_back(zerov);
						avatarConn->playLipsync("tts", "", filename,visemes );
						playSound(true, filename);
						//removeFile
						cout << "remove file " << filename << endl;
						if(!boost::filesystem::remove(filename)) {

						}
					}
				} catch(expression::exception::Exception& e) {
					e.setMark(__FILE__, __LINE__);
					e.printStackTrace();
				}
				visemes.clear();
			}
		}else{
			try{
				boost::uuids::uuid uuid = boost::uuids::random_generator()();
				stringstream filestream;
				filestream << "/tmp/" << uuid << ".wav";
				string filename = filestream.str();
				getSentenceInfo(sentence, filename, dirpath, visemes);
				//visemes.push_back(zerov);
				avatarConn->playLipsync("tts", "", filename,visemes );
				playSound(true, filename);
				//removeFile
				cout << "remove file " << filename << endl;
				if(!boost::filesystem::remove(filename)) {

				}
			}catch (expression::exception::Exception& e) {
				e.setMark(__FILE__, __LINE__);
				e.printStackTrace();
			}
		}
	} catch(expression::exception::IOException& e) {
		e.setMark(__FILE__, __LINE__);
		e.printStackTrace();
	}

	return true;
}


bool Lipsync::songPlay(string filename )
{
	cout << "Lipsync song filename [ "<< filename <<" ]"<< endl;

	try {
			cout << " filename: " << filename << endl;
			vector<silbot3_msgs::TTSViseme> visemes;

			avatarConn->playLipsync("song", "",filename, visemes);

			playSound(false, filename);

	} catch(expression::exception::IOException& e) {
		e.setMark(__FILE__, __LINE__);
		e.printStackTrace();
	}

	return true;
}


bool Lipsync::getSentenceInfo(const string& sentence, string& filename, string& dirpath,  vector<silbot3_msgs::TTSViseme>& visemes ) throw (expression::exception::IOException)
{
	bool result = false;
	try {

		soundConn->makeTTSWav(sentence, filename, visemes);
		result  = true;
		cout << "Lipsync::getSentenceInfo : " ;
		for(int i=0; i < visemes.size(); i++){
			cout << visemes[i].id << ",";
			cout << visemes[i].duration << "|";
		}
		cout << endl;
	} catch(expression::exception::IOException& e) {
		e.setMark(__FILE__, __LINE__);
		throw e;
	}

	return result;
}


void Lipsync::split(string str, vector<SentenceUnit>& result) {
	string text = str;
	char_separator<char> sep("+");
	tokenizer<char_separator<char> > tokens(text, sep);
	tokenizer<char_separator<char> >::const_iterator itr;
	string token;
	string sentenceToken;
	for(itr = tokens.begin(); itr != tokens.end(); itr++) {
		SentenceUnit word;
		try {
			token = *itr;
			sentenceToken = *itr;
			token.erase( std::remove_if( token.begin(), token.end(), ::isspace ), token.end() );
			lexical_cast<int>(token);
			word.isRecording = true;
			cout<<"recording true"<<endl;
		} catch(bad_lexical_cast& e) {
			token = sentenceToken;
			word.isRecording = false;
			cout<<"recording false"<<endl;
		}
		word.sentence = token;
		cout << word.sentence << endl;
		result.push_back(word);
	}

}

void Lipsync::playSound( bool blocking, const string& filename )
{
	ROS_INFO("play sound filename : %s",filename.c_str());
	soundConn->playSound(blocking, filename);
}
