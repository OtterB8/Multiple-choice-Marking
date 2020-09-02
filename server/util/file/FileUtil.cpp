#include <util/file/FileUtil.h>

FileUtil::FileUtil() {}

FileUtil* FileUtil::s_instance = 0;

FileUtil *FileUtil::instance() {
    if (!FileUtil::s_instance)
  		FileUtil::s_instance = new FileUtil;

    return FileUtil::s_instance;
}

bool FileUtil::isFile(string const &path) {
	struct stat s;
	return stat(path.c_str(),&s) == 0 && (s.st_mode & S_IFREG);
}

vector<string> FileUtil::listFiles(string const &path) {
	vector<string> res;
	
	DIR *dpdf = opendir(path.c_str());

	if (dpdf != NULL){
		struct dirent *epdf;
	   	while (epdf = readdir(dpdf)) {
	   		string absolutePath = path + epdf->d_name;

	   		if (!this->isFile(absolutePath))
	   			continue;

	      	res.push_back(absolutePath);
	   	}
	}

	closedir(dpdf);

	return res;
}

string FileUtil::getFileName(string const &path, bool withExtension) {
	int slashPos = path.rfind("/");
	
	if (withExtension)
		return path.substr(slashPos + 1);

	int dotPos = path.rfind(".");
	return path.substr(slashPos + 1, dotPos - slashPos - 1);
}

void FileUtil::createDirectory(string const &dir) {
	int status = mkdir(dir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}