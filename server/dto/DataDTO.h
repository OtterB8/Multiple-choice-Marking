#pragma once
#include <string>

using namespace std;

class DataDTO {
	public:
		virtual ~DataDTO() {}
		virtual string toJson() = 0;
};