#include <util/marking/MarkingUtilImpl.h>

unordered_map<int, vector<char>> MarkingUtilImpl::buildAnswers(const vector<string> &images) {
	unordered_map<int, vector<char>> answers;
	try {
		for (string image: images) {
			readAnswer(image.c_str(), answers);
		}
	} catch (const char *msg) {
		throw BuildAnswerException(msg);
	}
	return answers;
}

pair<int, int> MarkingUtilImpl::mark(const string &path, unordered_map<int, vector<char>> const &answers, bool shouldSaveMarkedImage, const string &pathSaveMarkedImage) {
	pair<int, int> result;
	Mat visualize;

	try {
		tie(result, visualize) = examiner(path.c_str(), answers);
		if (shouldSaveMarkedImage) {
			imwrite(pathSaveMarkedImage, visualize);
		}
	} catch (const char *msg) {
		throw MarkingException(msg);
	}

	return result;
}