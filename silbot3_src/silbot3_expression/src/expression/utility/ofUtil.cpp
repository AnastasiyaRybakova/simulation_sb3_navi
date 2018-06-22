#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <ctype.h>


#include "ofUtil.h"


ofUtil::ofUtil(void)
{
}

ofUtil::~ofUtil(void)
{
}

std::string ofUtil::ofToString( double value, int precision )
{
	stringstream sstr;
	sstr << fixed << setprecision(precision) << value;
	return sstr.str();

}

std::string ofUtil::ofToString( int value )
{
	stringstream sstr;
	sstr << value;
	return sstr.str();
}

float ofUtil::ofToFloat( const string& floatString )
{
	float x;
	sscanf(floatString.c_str(), "%f", &x);
	return x;
}

int ofUtil::ofToInt( const string& intString )
{
	int x;
	sscanf(intString.c_str(), "%d", &x);
	return x;

}

vector<string> ofUtil::tokenize( const string & str, const string & delim )
{
	vector<string> tokens;

	size_t p0 = 0, p1 = string::npos;
	while(p0 != string::npos)
	{
		p1 = str.find_first_of(delim, p0);
		if(p1 != p0)
		{
			string token = str.substr(p0, p1 - p0);
			tokens.push_back(token);
		}
		p0 = str.find_first_not_of(delim, p1);
	}
	return tokens;
}

float ofUtil::ofRandom( float x, float y )
{

	float high = 0;
	float low = 0;
	float randNum = 0;
	// if there is no range, return the value
	if (x == y) return x; 			// float == ?, wise? epsilon?
	high = MAX(x,y);
	low = MIN(x,y);
	return randNum;
}

bool ofUtil::is_number(const std::string& s)
{
	std::string::const_iterator it = s.begin();
	while (it != s.end() && isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}

void ofUtil::TrimSpaces( string& str )
{
	// Trim Both leading and trailing spaces
	size_t startpos = str.find_first_not_of(" \t"); // Find the first character position after excluding leading blank spaces
	size_t endpos = str.find_last_not_of(" \t"); // Find the first character position from reverse af

	// if all spaces or empty return an empty string
	if(( string::npos == startpos ) || ( string::npos == endpos))
	{
		str = "";
	}
	else
		str = str.substr( startpos, endpos-startpos+1 );

	/*
	// Code for Trim Leading Spaces only
	size_t startpos = str.find_first_not_of(�� \t��); // Find the first character position after excluding leading blank spaces
	if( string::npos != startpos )
	str = str.substr( startpos );
	*/

	/*
	// Code for Trim trailing Spaces only
	size_t endpos = str.find_last_not_of(�� \t��); // Find the first character position from reverse af
	if( string::npos != endpos )
	str = str.substr( 0, endpos+1 );
	*/

}

