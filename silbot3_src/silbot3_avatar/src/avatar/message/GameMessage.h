#ifndef __GAMEMESSAGE_H__
#define __GAMEMESSAGE_H__

#include "ParameterPart.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <robocare/exception/NoSuchElementException.h>
#include <robocare/exception/NullPointerException.h>

using namespace std;
using namespace roscir::exception;

typedef map<string, string> StringMap;
typedef pair<string, string> StringPair;

namespace avatar {
	namespace message{
		class GameMessage {

		private :
			StringMap* parameterMap;
			//StlMap<string, string> parameterMap;
			string __DEFAULT_VALUE;
			string messageType;
			string sessionID;
			string operationName;
			//ParameterPart parameters;

		public :
			GameMessage(void);
			GameMessage(const string& type, const string& sessionID, const string& operationName);
			virtual ~GameMessage(void);

		public :
			string toXML(void);
			void setMessageType(const string& type);
			void setOperationName(const string& name);
			void setSessionID(const string& sid);
			void addParameter(const string& name, const string& value);

			string getSessionID() throw (NullPointerException);
			string getParameter(const string& name) throw (NoSuchElementException);
			string getOperationName() throw (NullPointerException);
			vector<string> getParameterKeys() throw (NullPointerException);

		private :
			string makeParamString();
			string makeDataSetString();
		};
	}}
#endif
