#pragma once

#include <exception/BaseException.h>

class MarkingException : public BaseException {
	public:
		MarkingException(string cause): BaseException(cause) {}
};