#ifndef __EXPRESSION_EXCEPTION_IOEXCEPTION_H_
#define __EXPRESSION_EXCEPTION_IOEXCEPTION_H_

#include "Exception.h"

namespace expression {
namespace exception {


class IOException : public expression::exception::Exception {
public:
	IOException();
	virtual ~IOException() throw();
};

	} /* namespace exceptions */
} /* namespace expression */
#endif /* __EXPRESSION_EXCEPTION_IOEXCEPTION_H_ */
