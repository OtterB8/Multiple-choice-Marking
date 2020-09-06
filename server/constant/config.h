#pragma once
#include <string>
#include <thread>
#include <iostream>
using namespace std;

const int NUMBER_OF_PROCESSING_THREAD = thread::hardware_concurrency() == 0 ? 4 : thread::hardware_concurrency();

const string FOLDER_ANSWER = "DapAn/";
const string FOLDER_TEST = "BaiLam/";

const int NUMBER_OF_QUESTION = 40;