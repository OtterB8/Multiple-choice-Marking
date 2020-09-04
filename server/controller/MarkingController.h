#include <iostream>
#include <lib/external/httplib.h>
#include <handler/MarkingHandler.h>
#include <thread>
#include <dto/ResponseDTO.h>
#include <constant/ErrorCode/ErrorCode.h>

using namespace std;
using namespace httplib;

class MarkingController
{
    private:
        static MarkingController *s_instance;
        MarkingController();
    public:
        static MarkingController *instance();
        void doGet(Request const &req, Response &res);
        void doPost(Request const &req, Response &res, const ContentReader &content_reader);
};