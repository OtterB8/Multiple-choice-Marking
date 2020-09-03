#include <util/marking/MarkingUtilImpl.h>

unordered_map<int, vector<char>> MarkingUtilImpl::buildAnswers(const vector<string> &images) {
	unordered_map<int, vector<char>> answers;

	for (string image: images) {
		int check = readAnswer(image.c_str(), answers);
		if (check == -1) {
			throw BuildAnswerException("Exam code equals 0 or duplicated");
		}
		if (!check) {
			throw BuildAnswerException("Incorrect answer format");
		}
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