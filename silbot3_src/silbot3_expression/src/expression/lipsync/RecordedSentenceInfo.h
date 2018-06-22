#ifndef _EXPRESSION_LIPSYNC_RECORDED_SENTENCE_INFO_H_
#define _EXPRESSION_LIPSYNC_RECORDED_SENTENCE_INFO_H_

#include <string>

using namespace std;
namespace expression {
namespace lipsync {

class RecordedSentenceInfo {
public:
	int id;
	string text;
	string viseme;
	string wavfilename;
public:
	RecordedSentenceInfo();
	virtual ~RecordedSentenceInfo();


	const int& getId() const;
	const string& getText() const;
	const string& getViseme() const;
	const string& getWavname() const;

};

} /* namespace expression */
}
#endif /* _EXPRESSION_LIPSYNC_RECORDED_SENTENCE_INFO_H_ */
