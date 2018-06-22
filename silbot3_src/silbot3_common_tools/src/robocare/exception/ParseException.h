/*
 * ParseException.h
 *
 *  Created on: Aug 14, 2013
 *      Author: ijpark
 */

#ifndef __ROSCIR_EXCEPTION_PARSEEXCEPTION_H_
#define __ROSCIR_EXCEPTION_PARSEEXCEPTION_H_

#include "Exception.h"
#include <robocare/device/CirApi.h>

namespace roscir {
namespace exception {

class CIRAPI ParseException : public Exception {
public:
	ParseException();
	virtual ~ParseException() throw();
};

} /* namespace exception */
} /* namespace roscir */
#endif /* __ROSCIR_EXCEPTION_PARSEEXCEPTION_H_ */
