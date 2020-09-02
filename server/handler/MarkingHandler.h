#include <constant/MarkingStatus.h>
#include <dto/TestDTO.h>
#include <dto/MarkingDTO.h>
#include <constant/config.h>
#include <util/file/FileUtil.h>
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <unordered_map>
#include <future>
#include <util/marking/BlobDetector.h>
#include <util/marking/Process.h>
#include <exception/BuildAnswerException.h>
#include <exception/MarkingException.h>

using namespace std;

class MarkingHandler
{
    private:
        static MarkingHandler *s_instance;
        MarkingHandler();
    private:
        MarkingStatus status;
        vector<TestDTO> result;
        string message;
    private:
        unordered_map<int, vector<char>> buildAnswerFromImage(string const &path);
        vector<TestDTO> mark(string const &path, unordered_map<int, vector<char>> answers, bool shouldSaveMarkedImage);
        void actualMarking(string const &path);
    public:
        static MarkingHandler *instance();
        bool doMarking(string const &path);
        MarkingDTO *getData();
};