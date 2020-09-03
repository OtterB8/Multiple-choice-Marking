#pragma once

#include <exception/BaseException.h>

class BuildAnswerException : public BaseException {
	public:
		BuildAnswerException(string const &cause): BaseException(cause) {}
};