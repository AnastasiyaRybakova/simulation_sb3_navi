#ifndef __EXPRESSION_UTILITY_OFUTIL_H__
#define __EXPRESSION_UTILITY_OFUTIL_H__

#include <iostream>
#include <string>

#ifndef MIN
#define MIN(x,y) (((x) < (y)) ? (x) : (y))
#endif

#ifndef MAX
#define MAX(x,y) (((x) > (y)) ? (x) : (y))
#endif

using namespace std;

class ofUtil {
public:
	ofUtil(void);
	~ofUtil(void);

	static string ofToString(double value, int precision);
	static float ofToFloat(const string& floatString);
	static int ofToInt(const string& intString);
	static string ofToString(int value);
	static vector<string> tokenize(const string & str, const string & delim);
	static float ofRandom(float x, float y);
	static bool is_number(const std::string& s);
	static void TrimSpaces( string& str );
};
#endif /* __EXPRESSION_UTILITY_OFUTIL_H__ */
