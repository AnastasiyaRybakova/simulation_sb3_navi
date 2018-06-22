#ifndef _EXPRESSION_LIPSYNC_VISEME_INFO_H_
#define _EXPRESSION_LIPSYNC_VISEME_INFO_H_

#include <string>
#include <vector>
#include <sstream>


using namespace std;

namespace expression {
namespace lipsync {

class Viseme {
public:
	string vid;
	int duration;

public:
	string toString() {
		stringstream ss;
		ss<<"["<<vid<<",,,,"<<duration;

		return ss.str();
	}

	string toFormatString() {
		stringstream ss;
		ss<<this->vid<<",20000";
		return ss.str();
	}

};

class VisemeInfo {
public:
	static const int  __VISEMETYPE_SENTENCE;
	static const int  __VISEMETYPE_RECORD;

	int type;
	string viseme;
	string filename;

	vector<Viseme> visems;

public:
	VisemeInfo();
	virtual ~VisemeInfo();

	void tokenize();
	void split(string str, vector<Viseme>& result);
	string toString();
};

} /* namespace expression
 */
}
#endif /* _EXPRESSION_LIPSYNC_VISEME_INFO_H_ */
