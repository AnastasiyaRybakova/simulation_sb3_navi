#ifndef __MESSAGEPARSER_H__
#define __MESSAGEPARSER_H__

#include "GameMessage.h"
#include <robocare/exception/IOException.h>
#include <tinyxml.h>
#include "ParseException.h"

using namespace roscir::exception;
namespace avatar {
	namespace message {
		class MessageParser {

		public :
			MessageParser(void);
			virtual ~MessageParser(void);
//			GameMessage* parse(const string& inString);
			void parse(const char* inString, GameMessage* message) throw (ParseException);
			
		private :
			void parseParameter(GameMessage* message, TiXmlElement* ele) throw (ParseException);
			void parseDataSet(GameMessage* message, TiXmlElement* ele);
			
		};
	}}
#endif
