#include "VisemeInfo.h"
#include <boost/tokenizer.hpp>
using namespace boost;

using namespace expression::lipsync;

VisemeInfo::VisemeInfo() {
	type = __VISEMETYPE_SENTENCE;
}

VisemeInfo::~VisemeInfo() {
}

void VisemeInfo::tokenize() {
	split(this->viseme, this->visems);
}

void VisemeInfo::split(string str, vector<Viseme>& result) {
    string text = str;
    char_separator<char> sep("|");
    char_separator<char> sep2(",");
    tokenizer<char_separator<char> > tokens(text, sep);
    tokenizer<char_separator<char> >::const_iterator itr;


    int index = 0;
    for(itr = tokens.begin(); itr != tokens.end(); itr++) {
    	Viseme v;
    	tokenizer<char_separator<char> > tokens2(*itr, sep2);
    	tokenizer<char_separator<char> >::const_iterator itr2 = tokens2.begin();
    	v.vid = *itr2;
    	itr2++;
    	v.duration = atoi((*itr2).c_str());
    	index++;
    	result.push_back(v);
    }

    if(this->type == __VISEMETYPE_RECORD) {
    	vector<Viseme> tempv = this->visems;;
		for(unsigned int i= 1; i<tempv.size(); i++){
			this->visems[i].duration =tempv[i+1].duration - tempv[i].duration;
		}
		this->visems[this->visems.size() - 1].duration = 50;
    }

}
