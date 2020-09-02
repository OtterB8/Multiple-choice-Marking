#pragma once
#include <DataDTO.h>
#include <string>
#include <constant/ErrorCode/ErrorCode.h>

using namespace std;

class ResponseDTO: public DataDTO {
	private:
		ErrorCode errorCode;
		string message;
		DataDTO *data;
	public:
		ResponseDTO(ErrorCode errorCode, string const &message) {
			this->errorCode = errorCode;
			this->message = message;
			this->data = NULL;
		}

		ResponseDTO(ErrorCode errorCode, string const &message, DataDTO *data) {
			this->errorCode = errorCode;
			this->message = message;
			this->data = data;
		}

		virtual string toJson() {
			string res = "{\"errorCode\":" + to_string(errorCodeToInt(this->errorCode))
				+ ",\"message\":\"" + this->message + "\"";
			
			if (this->data != NULL) {
				res += ",\"data\":" + this->data->toJson();
			}

			res += "}";
			
			return res;
		}
};