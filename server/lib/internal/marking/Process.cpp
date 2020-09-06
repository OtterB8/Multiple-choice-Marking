#include "Process.h"

void getPosOfTokenOfAnswer(vector<pair<Point, pair<int, char>>> &posOfAnswer, int tokenNumber)
{
	for (int i = 0; i <= 3; i++)
		for (int j = 0; j <= 9; j++)
		{
			char c;
			switch (i)
			{
			case 0:
				c = 'A';
				break;
			case 1:
				c = 'B';
				break;
			case 2:
				c = 'C';
				break;
			default:
				c = 'D';
			}
			
			posOfAnswer.push_back(pair<Point, pair<int, char>>(Point(980 + 306 * (tokenNumber - 1) + 61 * i, 562 + 61 * j),
				pair<int, char>(10 * (tokenNumber - 1) + j + 1, c)));
		}
}

void getInformation(vector<pair<Point, int>> &posOfExamCode, vector<pair<Point, int>> &posOfCandidateNumber,
	vector<pair<Point, pair<int, char>>> &posOfAnswer)
{
	//Get posOfExamCode
	for (int i = 0; i <= 2; i++)
		for (int j = 0; j <= 9; j++)
			posOfExamCode.push_back(pair<Point, int>(Point(186 + 61 * i, 562 + 61 * j), j * pow(10, (2 - i))));
	
	//Get posOfExamCode
	for (int i = 0; i <= 5; i++)
		for (int j = 0; j <= 9; j++)
			posOfCandidateNumber.push_back(pair<Point, int>(Point(492 + 61 * i, 562 + 61 * j), j * pow(10, (5 - i))));

	//Get posOfAnswer
	for (int i = 1; i <= 4; i++)
		getPosOfTokenOfAnswer(posOfAnswer, i);
}

void readAnswer(const char* path, unordered_map<int, vector<char>> &answers)
{
	BlobDetector* blob = new BlobDetector;
	bool check = blob->LoadImage(path);
	if (!check)
	{
		throw "Incorrect path";
	}

	vector<Point> circles = blob->DetectBlob();

	vector<pair<Point, int>> posOfExamCode;
	vector<pair<Point, int>> posOfCandidateNumber;
	vector<pair<Point, pair<int, char>>> posOfAnswer;
	getInformation(posOfExamCode, posOfCandidateNumber, posOfAnswer);

	int examCode = 0;
	int countCircle = 0;
	//Get examCode
	for (int i = 0; i < circles.size(); i++)
		for (int j = 0; j < posOfExamCode.size(); j++)
		{
			Point temp = posOfExamCode[j].first;
			if (abs(circles[i].x - temp.x) <= deviation && abs(circles[i].y - temp.y) <= deviation)
			{
				examCode += posOfExamCode[j].second;
				countCircle++;
				break;
			}
		}

	if (countCircle != 3)
	{
		delete blob;
		throw "Exam code is invalid";
	}
	if (answers.find(examCode) != answers.end())
	{
		delete blob;
		throw "Exam code is already exist";
	}

	vector<pair<char, int>> answer;
	for (int i = 0; i < numberOfQuestion; i++)
		answer.push_back(pair<char, int>('e', 0));

	for (int i = 0; i < circles.size(); i++)
		for (int j = 0; j < posOfAnswer.size(); j++)
		{
			Point temp = posOfAnswer[j].first;
			if (abs(circles[i].x - temp.x) <= deviation && abs(circles[i].y - temp.y) <= deviation)
			{
				pair<int, char> t = posOfAnswer[j].second;
				pair<char, int> newAnswer = pair<char, int>(t.second, answer[t.first - 1].second + 1);
				answer[t.first - 1] = newAnswer;
				break;
			}
		}

	pair<int, vector<char>> res;
	res.first = examCode;
	for (int i = 0; i < answer.size(); i++)
	{
		if (!answer[i].second || answer[i].second > 1)
		{
			delete blob;
			throw "Answer is in incorrect format";
		}
		res.second.push_back(answer[i].first);
	}
	answers.insert(res);

	delete blob;
}

pair<pair<int, int>,Mat> examiner(const char* path, unordered_map<int, vector<char>> answers)
{
	BlobDetector* blob = new BlobDetector;
	bool check = blob->LoadImage(path);
	if (!check)
	{
		throw "Incorrect path";
	}

	vector<Point> circles = blob->DetectBlob();
	
	Mat visualize;
	vector<pair<Point, int>> posOfExamCode;
	vector<pair<Point, int>> posOfCandidateNumber;
	vector<pair<Point, pair<int, char>>> posOfAnswer;
	getInformation(posOfExamCode, posOfCandidateNumber, posOfAnswer);

	int examCode = 0, candidateNumber = 0, score = 0, countCircleOfExamCode = 0, countCircleOfCandidateNumber = 0;
	
	//Get candidateNumber and examCode
	for (int i = 0; i < circles.size(); i++)
	{
		for (int j = 0; j < posOfExamCode.size(); j++)
		{
			Point temp = posOfExamCode[j].first;
			if (abs(circles[i].x - temp.x) <= deviation && abs(circles[i].y - temp.y) <= deviation)
			{
				examCode += posOfExamCode[j].second;
				countCircleOfExamCode++;
				break;
				continue;
			}
		}

		for (int j = 0; j < posOfCandidateNumber.size(); j++)
		{
			Point temp = posOfCandidateNumber[j].first;
			if (abs(circles[i].x - temp.x) <= deviation && abs(circles[i].y - temp.y) <= deviation)
			{
				candidateNumber += posOfCandidateNumber[j].second;
				countCircleOfCandidateNumber++;
				break;
			}
		}
	}

	if (countCircleOfExamCode != 3 && countCircleOfCandidateNumber != 6)
	{
		throw "Exam code and candidate number are invalid";
	}
	if (countCircleOfExamCode != 3)
	{
		throw "Exam code is invalid";
	}
	if (countCircleOfCandidateNumber != 6)
	{
		throw "Candidate number is invalid";
	}

	//Get score
	vector<char> answer;
	if (answers.find(examCode) != answers.end())
	{
		answer = answers[examCode];

		vector<pair<char, int>> answerOfCandidate;
		for (int i = 0; i < answer.size(); i++)
			answerOfCandidate.push_back(pair<char, int>('e', 0));

		for (int i = 0; i < circles.size(); i++)
			for (int j = 0; j < posOfAnswer.size(); j++)
			{
				Point temp = posOfAnswer[j].first;
				if (abs(circles[i].x - temp.x) <= deviation && abs(circles[i].y - temp.y) <= deviation)
				{
					pair<int, char> t = posOfAnswer[j].second;
					pair<char, int> newAnswer = pair<char, int>(t.second, answerOfCandidate[t.first - 1].second + 1);
					answerOfCandidate[t.first - 1] = newAnswer;
					break;
				}
			}

		for (int i = 0; i < answerOfCandidate.size(); i++)
			if (answerOfCandidate[i].second == 1)
				if (toupper(answerOfCandidate[i].first) == toupper(answer[i]))
					score++;
	}

	//Visualize
	visualize = blob->ShowImage(answer, posOfAnswer);

	delete blob;
	return pair<pair<int, int>, Mat>(pair<int,int>(candidateNumber, score), visualize);
}