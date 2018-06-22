/*
 * ConnectException.h
 *
 *  Created on: Jul 17, 2013
 *      Author: ijpark
 */

#ifndef __EXPRESSION_EXCEPTION_CONNECTEXCEPTION_H_
#define __EXPRESSION_EXCEPTION_CONNECTEXCEPTION_H_

#include "Exception.h"

namespace expression {
namespace exception {

class ConnectException : public Exception {
public:
	ConnectException();
	virtual ~ConnectException() throw ();
};

} /* namespace exceptions */
} /* namespace avatar */

#endif /* __EXPRESSION_EXCEPTION_CONNECTEXCEPTION_H_ */
