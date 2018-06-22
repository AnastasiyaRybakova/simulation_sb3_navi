/*
 * NullPointerException.h
 *
 *  Created on: Jul 17, 2013
 *      Author: ijpark
 */

#ifndef __ROSCIR_EXCEPTION_NULLPOINTEREXCEPTION_H_
#define __ROSCIR_EXCEPTION_NULLPOINTEREXCEPTION_H_

#include "Exception.h"
#include <robocare/device/CirApi.h>

namespace roscir {
namespace exception {

class CIRAPI NullPointerException : public Exception {
public:
	NullPointerException();
	virtual ~NullPointerException() throw();
};

}} /* namespace exceptions */
#endif /* __ROSCIR_EXCEPTION_NULLPOINTEREXCEPTION_H_ */
