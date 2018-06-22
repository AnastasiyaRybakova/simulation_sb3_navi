/*
 * NoSuchElementException.h
 *
 *  Created on: Aug 12, 2013
 *      Author: ijpark
 */

#ifndef __ROSCIR_EXCEPTION_NOSUCHELEMENTEXCEPTION_H_
#define __ROSCIR_EXCEPTION_NOSUCHELEMENTEXCEPTION_H_

#include "Exception.h"
#include <robocare/device/CirApi.h>

namespace roscir {
namespace exception {

class CIRAPI NoSuchElementException : public Exception {
public:
	NoSuchElementException();
	virtual ~NoSuchElementException() throw ();
};

} /* namespace exception */
} /* namespace roscir */
#endif /* __ROSCIR_EXCEPTION_NOSUCHELEMENTEXCEPTION_H_ */
