// #include <iostream>
// #include <lib/external/httplib.h>
// #include <handler/MarkingHandler.h>
// #include <thread>
// #include <dto/ResponseDTO.h>
// #include <constant/ErrorCode/ErrorCode.h>

#include <controller/MarkingController.h>

// using namespace std;
// using namespace httplib;

int main() {
	Server svr;

	svr.Post("/mark", [&](const Request &req, Response &res, const ContentReader &content_reader) {
      	MarkingController::instance()->doPost(req, res, content_reader);
  	});

	svr.Get("/mark", [](const Request &req, Response &res) {
		MarkingController::instance()->doGet(req, res);
	});

	int port = 8080;
	cout << "Server is listening on port " << port << "\n";
	svr.listen("0.0.0.0", port);

	return 0;
}