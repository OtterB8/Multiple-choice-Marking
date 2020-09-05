#include <controller/MarkingController.h>

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