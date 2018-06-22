/*
 * TimeoutException.h
 *
 *  Created on: Aug 16, 2013
 *      Author: ijpark
 */

#ifndef __ROSCIR_EXCEPTION_TIMEOUTEXCEPTION_H_
#define __ROSCIR_EXCEPTION_TIMEOUTEXCEPTION_H_

#include "Exception.h"
#include <robocare/device/CirApi.h>

namespace roscir {
namespace exception {

class CIRAPI TimeoutException : public Exception {
public:
	TimeoutException();
	virtual ~TimeoutException() throw();
};

} /* namespace exception */
} /* namespace roscir */
#endif /* __ROSCIR_EXCEPTION_TIMEOUTEXCEPTION_H_ */
