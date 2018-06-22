#include "SentenceTokenizer.h"

namespace behavior {
namespace util {

string remove_ws(const string& source) {
	stringstream ss;
	for(unsigned int i = 0; i < source.length(); i++) {
		if(source.at(i) != ' ') {
			ss<<source.at(i);
		}
	}

	return ss.str();
}

SentenceTokenizer::SentenceTokenizer() {
}

SentenceTokenizer::~SentenceTokenizer() {
}

bool SentenceTokenizer::isNumericToken(const string& sentence) {
	bool result = false;
	string temp = remove_ws(sentence);
    try
    {
    	boost::lexical_cast<int>(temp);
    	result = true;
    }
    catch(boost::bad_lexical_cast &e)
    {
    	result = false;
    }

    return result;
}

void SentenceTokenizer::tokenize(const string& source, vector<string>& result) {
	 boost::char_separator<char> sep("+");
	 boost::tokenizer<boost::char_separator<char> > tokens(source, sep);
	 boost::tokenizer<boost::char_separator<char> >::const_iterator itr;
	 for(itr = tokens.begin(); itr != tokens.end(); itr++) {
		 result.push_back(*itr);
	 }
}

void SentenceTokenizer::tokenize(const string& source, vector<Token>& result) {
	 boost::char_separator<char> sep("+");
	 boost::tokenizer<boost::char_separator<char> > tokens(source, sep);
	 boost::tokenizer<boost::char_separator<char> >::const_iterator itr;
	 for(itr = tokens.begin(); itr != tokens.end(); itr++) {
		 string s = *itr;
		 struct Token tok;
		 try {
			 //int recordId = roscir::lang::Integer::valueOf(s);
			 int recordId = boost::lexical_cast<int>(s);

			 tok.type = TOKEN_TYPE_RECORD;
			 tok.recordid = recordId;
		 } catch(roscir::exception::NumberFormatException& e) {
			 tok.type = TOKEN_TYPE_SENTENCE;
			 tok.sentence = s;
		 }
		 result.push_back(tok);
	 }
	 if(result.size() == 1) {
		 cout << "result.size() == 1"<< endl;
		 int found = source.find("+");
		 cout << "found == " << found<< endl;
		 if(found == 0) {
			 cout << "found == 0" << endl;
			 result[0].type = TOKEN_TYPE_SENTENCE;
		 }
	 }
}

} /* namespace util */
} /* namespace behavior */
