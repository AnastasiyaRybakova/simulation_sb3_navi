#ifndef __PARSE_EXCEPTION__
#define __PARSE_EXCEPTION__
#include <robocare/exception/Exception.h>
#include <string>

using namespace std;
using namespace roscir::exception;
namespace avatar {
	namespace message {
		class ParseException : public Exception {
		public:
			ParseException(const string& file, const int& line, const string& message) {
				setMessage(message);
				setMark(file.c_str(), line);
			}
			virtual ~ParseException() throw(){}
		};
	}
}

#endif
