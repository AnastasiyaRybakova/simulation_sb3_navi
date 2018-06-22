#ifndef __EXPRESSION_EXCEPTION_NO_SUCH_ELEMENT_EXCEPTION_H_
#define __EXPRESSION_EXCEPTION_NO_SUCH_ELEMENT_EXCEPTION_H_

#include "Exception.h"

namespace expression {
namespace exception {

class NoSuchElementException : public expression::exception::Exception {
public:
	NoSuchElementException();
	virtual ~NoSuchElementException() throw ();
};

} /* namespace exception */
} /* namespace expression */
#endif /* __EXPRESSION_EXCEPTION_NO_SUCH_ELEMENT_EXCEPTION_H_ */
