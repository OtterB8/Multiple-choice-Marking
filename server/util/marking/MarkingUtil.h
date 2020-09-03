#pragma once

#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

class MarkingUtil {
	public:
		virtual ~MarkingUtil() {}
		virtual unordered_map<int, vector<char>> buildAnswers(const vector<string> &images) = 0;
		virtual pair<int, int> mark(const string &path, unordered_map<int, vector<char>> const &answers, bool shouldSaveMarkedImage, const string &pathSaveMarkedImage = "") = 0;
};