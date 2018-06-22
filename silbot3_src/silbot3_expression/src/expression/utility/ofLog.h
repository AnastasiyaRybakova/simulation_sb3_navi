#ifndef __EXPRESSION_UTILITY_OFLOG_H__
#define __EXPRESSION_UTILITY_OFLOG_H__

#include <stdio.h>
#include <iostream>
#include <string.h>

#include <log4cxx/logger.h>
#include <log4cxx/xml/domconfigurator.h>
#include <log4cxx/helpers/exception.h>


enum ofLogLevel{
	OF_LOG_VERBOSE,
	OF_LOG_NOTICE,
	OF_LOG_WARNING,
	OF_LOG_ERROR,
	OF_LOG_FATAL_ERROR,
	OF_LOG_SILENT	//this one is special and should always be last - set ofSetLogLevel to OF_SILENT to not recieve any messages
};

#define OF_DEFAULT_LOG_LEVEL  OF_LOG_NOTICE;

using namespace std;

using namespace log4cxx;
using namespace log4cxx::xml;
using namespace log4cxx::helpers;

class ofLog {

public:
	ofLog(void);
	~ofLog(void);

	static int currentLogLevel ;

	static void ofSetLogLevel(int logLevel);

	//static void write(int logLevel, string message);
	static void write(int logLevel, const char* format, ...);
	static void initLog4CXX();

	static string format_arg_list(const char *fmt, va_list args);
};

#endif /* __EXPRESSION_UTILITY_OFLOG_H__ */
