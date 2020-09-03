#pragma once

#include <DataDTO.h>
#include <TestDTO.h>
#include <constant/config.h>
#include <string>
#include <vector>

using namespace std;

class MarkingDTO: public DataDTO {
	private:
		int numberofquestions = NUMBER_OF_QUESTION;
		vector<TestDTO> listofpoints;
		MarkingStatus status;
		string message;
	public:
		MarkingDTO(vector<TestDTO> const &listofpoints, MarkingStatus status, string const &message) {
			this->listofpoints = listofpoints;
			this->status = status;
			this->message = message;
		}
		virtual string toJson() {
			string res = "{\"status\":" + to_string(markingStatusToInt(this->status))
						+ ",\"message\":\"" + this->message
						+ "\",\"data\":{\"numberofquestions\":" + to_string(this->numberofquestions) + ",\"listofpoints\":[";

			
			string split = "";
			for (auto &value: this->listofpoints) {
				res += split + value.toJson();
				split = ",";
			}

			res += "]}}";

			return res;
		}
};