/*
 * ConnectException.h
 *
 *  Created on: Jul 17, 2013
 *      Author: ijpark
 */

#ifndef __ROSCIR_EXCEPTION_CONNECTEXCEPTION_H_
#define __ROSCIR_EXCEPTION_CONNECTEXCEPTION_H_

#include "Exception.h"
#include <robocare/device/CirApi.h>

namespace roscir {
namespace exception {

class CIRAPI ConnectException : public Exception {
public:
	ConnectException();
	virtual ~ConnectException() throw ();
};

} /* namespace exceptions */
} /* namespace avatar */

#endif /* __ROSCIR_EXCEPTION_CONNECTEXCEPTION_H_ */


