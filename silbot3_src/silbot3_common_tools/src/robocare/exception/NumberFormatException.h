/*
 * NumberFormatException.h
 *
 *  Created on: Aug 9, 2013
 *      Author: ijpark
 */

#ifndef __ROSCIR_EXCEPTION_NUMBERFORMATEXCEPTION_H_
#define __ROSCIR_EXCEPTION_NUMBERFORMATEXCEPTION_H_

#include "Exception.h"
#include <robocare/device/CirApi.h>

namespace roscir {
namespace exception {

class CIRAPI NumberFormatException : public Exception {
public:
	NumberFormatException();
	virtual ~NumberFormatException() throw();
};

} /* namespace exception */
} /* namespace roscir */
#endif /* __ROSCIR_EXCEPTION_NUMBERFORMATEXCEPTION_H_ */
