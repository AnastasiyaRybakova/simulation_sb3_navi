/*
 * ArrayOutOfBoundException.h
 *
 *  Created on: Aug 12, 2013
 *      Author: ijpark
 */

#ifndef __ROSCIR_EXCEPTION_ARRAYOUTOFBOUNDEXCEPTION_H_
#define __ROSCIR_EXCEPTION_ARRAYOUTOFBOUNDEXCEPTION_H_

#include "Exception.h"
#include <robocare/device/CirApi.h>

namespace roscir {
namespace exception {

class CIRAPI ArrayOutOfBoundException : public Exception {
public:
	ArrayOutOfBoundException();
	virtual ~ArrayOutOfBoundException() throw();
};

} /* namespace exception */
} /* namespace roscir */
#endif /* __ROSCIR_EXCEPTION_ARRAYOUTOFBOUNDEXCEPTION_H_ */
