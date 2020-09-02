#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <dirent.h>
#include <sys/stat.h>

using namespace std;

class FileUtil
{
	private:
		static FileUtil *s_instance;
        FileUtil();
    public:
    	static FileUtil *instance();
    	vector<string> listFiles(string const &path);
    	bool isFile(string const &path);
    	string getFileName(string const &path, bool withExtension);
    	void createDirectory(string const &dir);
};