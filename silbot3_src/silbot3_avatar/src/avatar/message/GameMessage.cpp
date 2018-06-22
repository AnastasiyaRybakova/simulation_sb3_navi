#include "GameMessage.h"

using namespace avatar::message;

GameMessage::GameMessage(void)
{
	this->parameterMap = new StringMap;
	this->__DEFAULT_VALUE	= "not_bound";
	this->messageType		= "GAME_MESSAGE";
	this->sessionID			= this->__DEFAULT_VALUE;
	this->operationName		= this->__DEFAULT_VALUE;
}

GameMessage::GameMessage(const string& type, const string& id, const string& opName)
{
	this->parameterMap = new StringMap;
	this->__DEFAULT_VALUE	= "not_bound";
	this->messageType		= type;
	this->sessionID			= id;
	this->operationName		= opName;
}

GameMessage::~GameMessage(void)
{
	delete this->parameterMap;
}

void GameMessage::setMessageType(const string& type) 
{
	this->messageType = type;
}

void GameMessage::setOperationName(const string& name)
{
	this->operationName = name;
}

void GameMessage::setSessionID(const string& sid)
{
	this->sessionID = sid;
}

string GameMessage::getSessionID() throw (NullPointerException)
{
	return this->sessionID;
}

void GameMessage::addParameter(const string& name, const string& value)
{
	// new Parameter() & add
	//parameterMap->put(name, value);
	parameterMap->insert(StringPair(name, value));
}

string GameMessage::getParameter(const string& name) throw (NoSuchElementException)
{
	StringMap::const_iterator itr = parameterMap->find(name);
	if(itr != parameterMap->end()) {
		return itr->second;
	} else {
		stringstream ss;
		ss<<"NosuchElemntException occurred ["<<name<<"]";
		NoSuchElementException e;
		e.setMessage(ss.str());
		e.setMark(__FILE__, __LINE__);
		throw e;
	}
}

vector<string> GameMessage::getParameterKeys() throw (NullPointerException)
{
	vector<string> keyset;
	StringMap::const_iterator itr;
	for(itr = parameterMap->begin(); itr != parameterMap->end(); itr++) {
		keyset.push_back(itr->first);
	}
	return keyset;
}

string GameMessage::getOperationName() throw (NullPointerException)
{
	return this->operationName;
}

string GameMessage::toXML(void)
{
	//cout<<"[GameMessage] toXML" <<endl;
	string str = "<game-message> \n";
		str += "<type value=\"GAME_MESSAGE\"/> \n";
		str += "<session id=\""; str += this->sessionID;  str += "\"/> \n";
		str += "<operation name=\""; str += this->operationName;  str += "\"/> \n";
		str += makeParamString();
		str += makeDataSetString();
		str += "</game-message> \n";
		//printf("parser %08x \n", &str);
		//cout<<str<<endl;
	return str;
}

string GameMessage::makeParamString()
{
	
	string key = "";
	string pValue = "";
	string result = "<parameters>";
	vector<string> keySet =getParameterKeys();
	for( int i=0 ; i<keySet.size() ; i++){
		key = keySet.at(i);
		pValue = getParameter(key);
		result += "\n\t<parameter name =\"";
		result += key;
		result += "\">\n";
		result += "\t\t<value>";
		result += pValue;
		result += "</value>\n\t</parameter>";
	}
	result += "\n</parameters>\n";
	return result;
	
	
}

string GameMessage::makeDataSetString()
{
	
	string result = "<dataset>\n";

	result += "<rows>\n";
	result += "</rows>\n";

	result += "</dataset>\n";
	return result;
	
}
