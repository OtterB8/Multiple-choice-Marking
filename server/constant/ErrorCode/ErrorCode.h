#pragma once
#include <unordered_map>
#include <util/common.h>

using namespace std;

enum class ErrorCode {SUCCESS, FAILED};

static unordered_map<ErrorCode, int, EnumClassHash> errorCodeMapping = {
	{ErrorCode::FAILED, -1},
	{ErrorCode::SUCCESS, 0}
};

int errorCodeToInt(ErrorCode code);
string errorCodeToString(ErrorCode code);