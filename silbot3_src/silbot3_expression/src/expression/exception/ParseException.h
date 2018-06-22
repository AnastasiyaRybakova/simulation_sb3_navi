#ifndef __EXPRESSION_EXCEPTION_PARSE_EXCEPTION_H_
#define __EXPRESSION_EXCEPTION_PARSE_EXCEPTION_H_

#include "Exception.h"

namespace expression {
namespace exception {

class ParseException : public expression::exception::Exception {
public:
	ParseException();
	virtual ~ParseException() throw();
};

} /* namespace exception */
} /* namespace expression */
#endif /* __EXPRESSION_EXCEPTION_PARSE_EXCEPTION_H_ */
