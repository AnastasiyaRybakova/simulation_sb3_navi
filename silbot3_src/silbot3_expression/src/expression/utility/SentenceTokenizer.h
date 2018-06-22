#ifndef __EXPRESSION_UTILITY_SENTENCETOKENIZER_H__
#define __EXPRESSION_UTILITY_SENTENCETOKENIZER_H__

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <sstream>

#include <robocare/exception/NumberFormatException.h>

#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace roscir::exception;

namespace behavior {
namespace util {

enum  TOKEN_TYPE {
	TOKEN_TYPE_SENTENCE,	/* sentence */
	TOKEN_TYPE_RECORD /* recoreded sentence */
};


struct Token {
	TOKEN_TYPE type;
	string sentence;
	int recordid;
};

class SentenceTokenizer {
private:
public:
	SentenceTokenizer();
	virtual ~SentenceTokenizer();

	void tokenize(const string& source, vector<string>& result);
	void tokenize(const string& source, vector<Token>& result);
	bool isNumericToken(const string& s);
};

} /* namespace util */
} /* namespace behavior */
#endif /* __EXPRESSION_UTILITY_SENTENCETOKENIZER_H__ */
