#ifndef __EXPRESSION_INVALID_PROTOCOL_EXCEPTION_H__
#define __EXPRESSION_INVALID_PROTOCOL_EXCEPTION_H__

#include "Exception.h"

namespace expression {
namespace exception {

class InvalidProtocolException : public expression::exception::Exception {

public:
	InvalidProtocolException();
	virtual ~InvalidProtocolException() throw();
};

}
} /* namespace expression */
#endif /* __EXPRESSION_INVALID_PROTOCOL_EXCEPTION_H__ */
