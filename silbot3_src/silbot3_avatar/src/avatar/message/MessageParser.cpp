#include "MessageParser.h"
#include <stdio.h>
#include <iostream>

using namespace avatar::message;

MessageParser::MessageParser(void)
{

}

MessageParser::~MessageParser(void)
{

}

void MessageParser::parse(const char* inString, GameMessage* message) throw (ParseException)
{
	TiXmlDocument* document = NULL;
	try
	{
	//	cout<<"[Message Parser] inString : "<<inString<<endl;
		string xmlString(inString);

		document = new TiXmlDocument();
		const char* result = document->Parse(xmlString.c_str());
		if(!result){
			exception e;
			throw &e;
			printf("%s\n", inString);
		}
		TiXmlElement* root = document->RootElement();
		string messageType = root->Value();
		//cout<<root->Value()<<endl;


		TiXmlElement* child = root->FirstChildElement();
		while(  child != NULL  ){

			string pName = child->Value();
			if( pName == "type" ) {
				//cout<<"type"<<endl;
				string type = child->Attribute("value");
				//cout<<"attribute : "<< type <<endl;
				message->setMessageType(type);

			} else if( pName == "session") {
				//cout<<"session"<<endl;
				string session = child->Attribute("id");
				//cout<<"attribute : "<< session <<endl;
				message->setSessionID(session);

			} else if( pName == "operation") {
				//cout<<"operation"<<endl;
				string operation = child->Attribute("name");
				//cout<<"attribute : "<< operation <<endl;
				message->setOperationName(operation);

			} else if( pName == "parameters") {
				//cout<<"parameters"<<endl;
				parseParameter(message, child);

			} else if( pName == "dataset") {
				//cout<<"dataset"<<endl;
				//parseDataSet();

			} else {
			}
			child = child->NextSiblingElement();
		}
		if(document) {
			delete document;
			document = NULL;
		}
	} catch(NullPointerException& e) {
		e.setMark(__FILE__, __LINE__);
		cout<<"[Nullpoint Exception]"<<inString<<endl;
		e.printStackTrace();
		if(document) {
			delete document;
			document = NULL;
		}

		ParseException pe(__FILE__, __LINE__, "ParseException occured");
		throw pe;
	} catch(exception* e) {
		if(document) {
			delete document;
			document = NULL;
		}
		//delete e;
		ParseException pe(__FILE__, __LINE__, "ParseException occured");
		throw pe;

	}

	// cout<<"parse end!!"<<endl;
}

void MessageParser::parseParameter(GameMessage* message, TiXmlElement* ele) throw (ParseException) {

	TiXmlElement* child = ele->FirstChildElement();
	try
	{
		while( child != NULL ){

			if(child->Attribute("name") == NULL || child->FirstChildElement()->GetText() == NULL){
				return;
			}

			message->addParameter( child->Attribute("name"), child->FirstChildElement()->GetText() );
			child = child->NextSiblingElement();
		}
	}
	catch (IOException& e)
	{
		ParseException pe(__FILE__, __LINE__, "ParseException occured");
		throw pe;
	}
	catch (NullPointerException& e)
	{
		ParseException pe(__FILE__, __LINE__, "ParseException occured");
		throw pe;
	}
}

void MessageParser::parseDataSet(GameMessage* message, TiXmlElement* ele){

}


/*
void MessageParser::test(const int& i, const string& s) {
	printf("%08x %d, %08x %s\n", &i, i, &s, s.c_str());
}

void MessageParser::test(const int& i, const char* s) {
	printf("zzzzzzzzzzz %08x %d, %08x %s\n", &i, i, &s, s);
}
*/
