#pragma once

#include <exception/BaseException.h>

class BuildAnswerException : public BaseException {
	public:
		BuildAnswerException(string cause): BaseException(cause) {}
};