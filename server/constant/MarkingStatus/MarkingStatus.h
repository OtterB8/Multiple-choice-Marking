#pragma once

#include <unordered_map>
#include <util/common.h>

using namespace std;

enum class MarkingStatus {PENDING, FAILED, SUCCESS, IDLE};

static unordered_map<MarkingStatus, int, EnumClassHash> markingStatusMapping = {
	{MarkingStatus::FAILED, -1},
	{MarkingStatus::SUCCESS, 0},
	{MarkingStatus::PENDING, 1},
	{MarkingStatus::IDLE, 2}
};

int markingStatusToInt(MarkingStatus status);
string markingStatusToString(MarkingStatus status);
