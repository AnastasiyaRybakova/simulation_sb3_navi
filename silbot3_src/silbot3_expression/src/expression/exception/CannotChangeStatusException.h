#ifndef __EXPRESSION_CANNOT_CHANGE_STATUS_EXCEPTION_H__
#define __EXPRESSION_CANNOT_CHANGE_STATUS_EXCEPTION_H__

#include "Exception.h"

namespace expression {
namespace exception {

	class CannotChangeStatusException : public expression::exception::Exception {
	public:
		CannotChangeStatusException();
		virtual ~CannotChangeStatusException() throw();
	};

} /* namespace exception */
} /* namespace expression */
#endif /* __EXPRESSION_CANNOT_CHANGE_STATUS_EXCEPTION_H__ */
