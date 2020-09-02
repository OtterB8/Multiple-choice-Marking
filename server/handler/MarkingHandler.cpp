#include <MarkingHandler.h>
#include <unistd.h>

MarkingHandler::MarkingHandler() {
	this->status = MarkingStatus::IDLE;
}

MarkingHandler* MarkingHandler::s_instance = 0;

MarkingHandler *MarkingHandler::instance() {
    if (!MarkingHandler::s_instance)
  		MarkingHandler::s_instance = new MarkingHandler;

    return MarkingHandler::s_instance;
}

MarkingDTO *MarkingHandler::getData() {
	switch (this->status) {
		case MarkingStatus::PENDING:
			return new MarkingDTO(vector<TestDTO>(), this->status, "In processing");
		case MarkingStatus::FAILED:
			return new MarkingDTO(vector<TestDTO>(), this->status, this->message);
		case MarkingStatus::SUCCESS:
			return new MarkingDTO(this->result, this->status, "Success");
		case MarkingStatus::IDLE:
			return new MarkingDTO(vector<TestDTO>(), this->status, "Idling");
	}
}

unordered_map<int, vector<char>> MarkingHandler::buildAnswerFromImage(string const &path) {
	vector<string> answerImageFiles = FileUtil::instance()->listFiles(path);
	
	int n = answerImageFiles.size();
	int batchNum = n / NUMBER_OF_PROCESSING_THREAD + 1;

	unordered_map<int, vector<char>> res;

	vector<future<unordered_map<int, vector<char>>>> futures;
	for (int i = 0; i < NUMBER_OF_PROCESSING_THREAD; ++i) {
		vector<string> batch;
		for (int j = 0; j < batchNum && j + i * batchNum <  n; ++j) {
			batch.push_back(answerImageFiles[j + i * batchNum]);
		}
		
		if (batch.size() == 0)
			break;

		futures.push_back(async(launch::async, [batch, this]() {
			unordered_map<int, vector<char>> answers;
			for (string image: batch) {
				int check = readAnswer(image.c_str(), answers);
				if (check == -1) {
					throw BuildAnswerException("Exam code equals 0 or duplicated");
				}
				if (!check) {
					throw BuildAnswerException("Incorrect answer format");
				}
			}
			return answers;
		}));
	}
	
	try {
		for (auto &ft: futures) {
			unordered_map<int, vector<char>> sub = ft.get();
			res.insert(sub.begin(), sub.end());
		}
	} catch(BuildAnswerException &e) {
		throw e;
	}
	
	return res;
}

vector<TestDTO> MarkingHandler::mark(string const &path, unordered_map<int, vector<char>> answers, bool shouldSaveMarkedImage) {
	string markedImagesPath = path + "marked/";
	FileUtil::instance()->createDirectory(markedImagesPath);

	vector<string> testImageFiles = FileUtil::instance()->listFiles(path);
	
	int n = testImageFiles.size();
	int batchNum = n / NUMBER_OF_PROCESSING_THREAD + 1;

	vector<TestDTO> res;

	vector<future<vector<TestDTO>>> futures;
	for (int i = 0; i < NUMBER_OF_PROCESSING_THREAD; ++i) {
		vector<string> batch;
		for (int j = 0; j < batchNum && j + i * batchNum <  n; ++j) {
			batch.push_back(testImageFiles[j + i * batchNum]);
		}
		
		if (batch.size() == 0)
			break;

		futures.push_back(async(launch::async, [batch, answers, markedImagesPath]() {
			vector<TestDTO> marked;
			for (string image: batch) {
				pair<int, int> result;
				Mat visualize;
				try {
					tie(result, visualize) = examiner(image.c_str(), answers);
					marked.push_back(
						TestDTO(result.first, FileUtil::instance()->getFileName(image, false), result.second));
					imwrite(markedImagesPath + FileUtil::instance()->getFileName(image, true), visualize);
				} catch (const char *msg) {
					throw MarkingException(msg);
				}
			}
			return marked;
		}));
	}
	
	try {
		for (auto &ft: futures) {
			vector<TestDTO> sub = ft.get();
			res.insert(res.end(), sub.begin(), sub.end());
		}
	} catch (MarkingException &e) {
		throw e;
	}
	
	return res;
}

void MarkingHandler::actualMarking(string const &path) {
	string answerPath = path + "/" + FOLDER_ANSWER;
	string testPath = path + "/" + FOLDER_TEST;

	unordered_map<int, vector<char>> answers;
	try {
		answers = this->buildAnswerFromImage(answerPath);
	} catch (BuildAnswerException &e) {
		this->message = e.what();
		this->status = MarkingStatus::FAILED;
		return;
	}

	vector<TestDTO> result;
	try {
		result = this->mark(testPath, answers, true);
	} catch (MarkingException &e) {
		this->message = e.what();
		this->status = MarkingStatus::FAILED;
		return;
	}

	this->result = result;
	this->status = MarkingStatus::SUCCESS;
}

bool MarkingHandler::doMarking(string const &path) {
	if (this->status == MarkingStatus::PENDING)
		return false;
	
	this->status = MarkingStatus::PENDING;
	
	thread t([this, path]() {
		this->actualMarking(path);
	});
	t.detach();

	return true;
}