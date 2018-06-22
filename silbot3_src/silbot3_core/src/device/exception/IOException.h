/*
 * IOException.h
 *
 *  Created on: Jul 16, 2013
 *      Author: ijpark
 */

#ifndef __ROSCIR_EXCEPTION_IOEXCEPTION_H_
#define __ROSCIR_EXCEPTION_IOEXCEPTION_H_

#include "Exception.h"

class  IOException : public Exception {
public:
	IOException();
	virtual ~IOException() throw();
};

#endif /* __ROSCIR_EXCEPTION_IOEXCEPTION_H_ */
