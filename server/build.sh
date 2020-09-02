echo "Building..."
export LD_RUN_PATH="/usr/local/lib"
g++ -std=c++11 util/marking/Process.cpp util/file/FileUtil.cpp constant/ErrorCode/ErrorCode.cpp constant/MarkingStatus/MarkingStatus.cpp handler/MarkingHandler.cpp server.cpp -o server.exe -I. -Ihandler -Idto -Iutil -Iexception -I/usr/local/include/opencv4/ -lpthread -lopencv_core -lopencv_imgproc -lopencv_imgcodecs
echo "Done"