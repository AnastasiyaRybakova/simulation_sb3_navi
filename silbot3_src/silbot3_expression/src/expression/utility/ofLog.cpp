#include "ofLog.h"
#include <iostream>
#include <stdio.h>
#include <stdarg.h>

using namespace std;

int ofLog::currentLogLevel = OF_DEFAULT_LOG_LEVEL;

LoggerPtr logger(Logger::getLogger("Behavior"));


ofLog::ofLog(void)
{

}

void ofLog::write( int logLevel, const char* format, ... )
{
	if(logLevel >= currentLogLevel){
		
		va_list args;
		va_start( args, format );

		if(logLevel == OF_LOG_VERBOSE){
			printf("OF_VERBOSE: ");
			std::string s = format_arg_list(format, args);
			va_end( args );
			LOG4CXX_INFO(logger, s);

		}
		else if(logLevel == OF_LOG_NOTICE){
			printf("OF_NOTICE: ");
			std::string s = format_arg_list(format, args);
			va_end( args );
			LOG4CXX_INFO(logger, s);

		}
		else if(logLevel == OF_LOG_WARNING){
			printf("OF_WARNING: ");
			std::string s = format_arg_list(format, args);
			va_end( args );
			LOG4CXX_INFO(logger, s);

		}
		else if(logLevel == OF_LOG_ERROR){
			printf("OF_ERROR: ");
			std::string s = format_arg_list(format, args);
			va_end( args );
			LOG4CXX_ERROR(logger, s);

		}
		else if(logLevel == OF_LOG_FATAL_ERROR){
			printf("OF_FATAL_ERROR: ");
			std::string s = format_arg_list(format, args);
			va_end( args );
			LOG4CXX_ERROR(logger, s);

		}


	}
}

//void ofLog::write( int logLevel, string message )
//{
//	if(logLevel >= currentLogLevel){
//		if(logLevel == OF_LOG_VERBOSE){
//			printf("OF_VERBOSE: ");
//		}
//		else if(logLevel == OF_LOG_NOTICE){
//			printf("OF_NOTICE: ");
//		}
//		else if(logLevel == OF_LOG_WARNING){
//			printf("OF_WARNING: ");
//		}
//		else if(logLevel == OF_LOG_ERROR){
//			printf("OF_ERROR: ");
//		}
//		else if(logLevel == OF_LOG_FATAL_ERROR){
//			printf("OF_FATAL_ERROR: ");
//		}
//		printf("%s\n",message.c_str());
//	}
//}

ofLog::~ofLog(void)
{
}

void ofLog::ofSetLogLevel( int logLevel )
{
	currentLogLevel = logLevel;
}

void ofLog::initLog4CXX()
{
	log4cxx::xml::DOMConfigurator::configure("C:\\home\\silvermate-win\\system\\config\\LogConfig.xml");
}

std::string ofLog::format_arg_list( const char *fmt, va_list args )
{
	if (!fmt) return "";
	int   result = -1, length = 256;
	char *buffer = 0;
	while (result == -1)
	{
		if (buffer) delete [] buffer;
		buffer = new char [length + 1];
		memset(buffer, 0, length + 1);
		result = snprintf(buffer, length, fmt, args);
		length *= 2;
	}
	std::string s(buffer);
	delete [] buffer;
	return s;
}
