#include <iostream>
#include <lib/httplib.h>
#include <handler/MarkingHandler.h>
#include <thread>
#include <dto/ResponseDTO.h>
#include <constant/ErrorCode/ErrorCode.h>

using namespace std;
using namespace httplib;

int main() {
	Server svr;

	svr.Post("/mark",
	  [&](const Request &req, Response &res, const ContentReader &content_reader) {
      	string body;
      	content_reader([&](const char *data, size_t data_length) {
        	body.append(data, data_length);
        	return true;
      	});
      	cout << "Receive post request " << body << endl;

      	bool isSuccess = MarkingHandler::instance()->doMarking(body);

      	if (isSuccess) {
      		res.set_content(
      			(new ResponseDTO(ErrorCode::SUCCESS, "Success"))->toJson(),
      			"application/json");
      	} else {
			res.set_content(
      			(new ResponseDTO(ErrorCode::FAILED, "In processing"))->toJson(),
      			"application/json");
      	}
	  });

	svr.Get("/mark", [](const httplib::Request &, httplib::Response &res) {
		cout << "Receive get request " << endl;
		DataDTO *data = MarkingHandler::instance()->getData();
	  	res.set_content(
	  		(new ResponseDTO(ErrorCode::SUCCESS, "Success", data))->toJson(),
	  		"application/json");
	  	delete data;
	});

	int port = 8080;

	cout << "Server is listening on port " << port << "\n";

	svr.listen("0.0.0.0", port);

	return 0;
}