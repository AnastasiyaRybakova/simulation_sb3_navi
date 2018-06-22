#ifndef __EXPRESSION_CANNOT_PLAY_EXCEPTION_H__
#define __EXPRESSION_CANNOT_PLAY_EXCEPTION_H__

#include "Exception.h"

namespace expression {
namespace exception {

	class CannotPlayException : public expression::exception::Exception {
	public:
		CannotPlayException();
		virtual ~CannotPlayException() throw();
	};

} /* namespace exception */
} /* namespace expression */
#endif /* __EXPRESSION_CANNOT_PLAY_EXCEPTION_H__ */
