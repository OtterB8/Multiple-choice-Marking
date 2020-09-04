echo "Building server..."
export LD_RUN_PATH="/usr/local/lib"
g++ -std=c++11 lib/internal/marking/Process.cpp util/marking/MarkingUtilImpl.cpp util/file/FileUtil.cpp constant/ErrorCode/ErrorCode.cpp constant/MarkingStatus/MarkingStatus.cpp handler/MarkingHandler.cpp controller/MarkingController.cpp server.cpp -o server.exe -I. -Ihandler -Idto -Iutil -Iexception -Icontroller -I/usr/local/include/opencv4/ -lpthread -lopencv_core -lopencv_imgproc -lopencv_imgcodecs
echo "Done building server"