#pragma once
#include "BlobDetector.h"
#include<unordered_map>

const int deviation = 12;
const int numberOfQuestion = 40;

void readAnswer(const char* path, unordered_map<int, vector<char>> &answers);
pair<pair<int, int>, Mat> examiner(const char* path, unordered_map<int, vector<char>> answers);