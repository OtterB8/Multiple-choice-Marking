#pragma once

#include <util/marking/MarkingUtil.h>
#include <lib/internal/marking/BlobDetector.h>
#include <lib/internal/marking/Process.h>
#include <exception/BuildAnswerException.h>
#include <exception/MarkingException.h>

using namespace std;

class MarkingUtilImpl: public MarkingUtil {
	public:
		unordered_map<int, vector<char>> buildAnswers(const vector<string> &images);
		pair<int, int> mark(const string &path, unordered_map<int, vector<char>> const &answers, bool shouldSaveMarkedImage, const string &pathSaveMarkedImage = "");
};