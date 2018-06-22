/*
 * FileNotFoundException.h
 *
 *  Created on: Aug 12, 2013
 *      Author: ijpark
 */

#ifndef __ROSCIR_EXCEPTION_FILENOTFOUNDEXCEPTION_H_
#define __ROSCIR_EXCEPTION_FILENOTFOUNDEXCEPTION_H_

#include "Exception.h"
#include <robocare/device/CirApi.h>

namespace roscir {
namespace exception {

class CIRAPI FileNotFoundException : public Exception {
public:
	FileNotFoundException();
	virtual ~FileNotFoundException() throw ();
};

} /* namespace exception */
} /* namespace roscir */
#endif /* __ROSCIR_EXCEPTION_FILENOTFOUNDEXCEPTION_H_ */
