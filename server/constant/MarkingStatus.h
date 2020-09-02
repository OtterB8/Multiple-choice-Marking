#pragma once

#include <unordered_map>
#include <util/common.h>

using namespace std;

enum class MarkingStatus {PENDING, FAILED, SUCCESS, IDLE};

static unordered_map<MarkingStatus, int, EnumClassHash> markingStatusMapping;

int markingStatusToInt(MarkingStatus status);
string markingStatusToString(MarkingStatus status);