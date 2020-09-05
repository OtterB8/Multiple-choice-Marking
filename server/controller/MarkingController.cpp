#include <MarkingController.h>
#include <unistd.h>

MarkingController::MarkingController() {}

MarkingController* MarkingController::s_instance = 0;

MarkingController *MarkingController::instance() {
    if (!MarkingController::s_instance)
  		MarkingController::s_instance = new MarkingController;

    return MarkingController::s_instance;
}

void MarkingController::doGet(Request const &req, Response &res) {
	DataDTO *data = MarkingHandler::instance()->getData();
	res.set_header("Access-Control-Allow-Origin", "*");
  	res.set_content(
  		(ResponseDTO(ErrorCode::SUCCESS, "Success", data)).toJson(),
  		"application/json");
  	delete data;
}

void MarkingController::doPost(Request const &req, Response &res, const ContentReader &content_reader) {
	string body;
	content_reader([&](const char *data, size_t data_length) {
  	body.append(data, data_length);
  	return true;
	});
  
	bool isSuccess = MarkingHandler::instance()->doMarking(body);

	res.set_header("Access-Control-Allow-Origin", "*");
	if (isSuccess) {
		res.set_content(
			(ResponseDTO(ErrorCode::SUCCESS, "Success")).toJson(),
			"application/json");
	} else {
	res.set_content(
			(ResponseDTO(ErrorCode::FAILED, "In processing")).toJson(),
			"application/json");
	}
}
