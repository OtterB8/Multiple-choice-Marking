#include <constant/ErrorCode/ErrorCode.h>

int errorCodeToInt(ErrorCode code) {
	return errorCodeMapping[code];
}

string errorCodeToString(ErrorCode code) {
	return to_string(errorCodeMapping[code]);
}