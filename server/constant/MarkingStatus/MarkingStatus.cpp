#include <constant/MarkingStatus/MarkingStatus.h>

int markingStatusToInt(MarkingStatus status) {
	return markingStatusMapping[status];
}

string markingStatusToString(MarkingStatus status) {
	return to_string(markingStatusMapping[status]);
}
