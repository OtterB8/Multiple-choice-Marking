#pragma once

#include <DataDTO.h>
#include <string>

using namespace std;

class TestDTO: public DataDTO {
	private:
		int id;
		string name;
		int numRight;
	public:
		TestDTO(int id, string const &name, int numRight) {
			this->id = id;
			this->name = name;
			this->numRight = numRight;
		}
		virtual string toJson() {
			string res = "{\"name\":\"" + this->name
				+ "\",\"identification\":" + to_string(this->id)
				+ ",\"numberrightanswer\":" + to_string(this->numRight)
				+ "}";
			return res;
		}
};