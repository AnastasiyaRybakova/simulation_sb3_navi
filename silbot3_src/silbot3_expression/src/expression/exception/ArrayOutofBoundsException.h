/*
 * ArrayOutofBoundsException.h
 *
 *  Created on: Jul 24, 2013
 *      Author: ijpark
 */

#ifndef ARRAYOUTOFBOUNDSEXCEPTION_H_
#define ARRAYOUTOFBOUNDSEXCEPTION_H_

#include "Exception.h"

namespace expression {
namespace exception {
	class ArrayOutofBoundsException : public Exception {
	public:
		ArrayOutofBoundsException();
		virtual ~ArrayOutofBoundsException() throw();
	};
}
} /* namespace expression */
#endif /* ARRAYOUTOFBOUNDSEXCEPTION_H_ */
