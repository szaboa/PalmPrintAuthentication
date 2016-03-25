#include <module_PalmImageReader\PalmReaderEnrollment.h>
#include <boost\filesystem.hpp>
#include <utility\PPAException.h>
#include <logger\Logger.h>
#include <string>

using namespace std;
using namespace boost::filesystem;

PalmReaderEnrollment::PalmReaderEnrollment(){}

void PalmReaderEnrollment::init(std::string dirPath) {
	this->dirPath = dirPath;
	path path(dirPath);
	for (auto i = directory_iterator(path); i != directory_iterator(); ++i){
		imageNames.push_back(i->path().filename().string());
	}
}

cv::Mat PalmReaderEnrollment::readPalmImage() {
	if (!hasNextImage()) throw PPAException("Error during reading images");
	
	cv::Mat input = cv::imread(dirPath + "/" + imageNames.at(iterator));
	
	if (!input.data) {
		throw PPAException("Error during reading images");
	} else{
		Logger::log("Reader", "Read: " + imageNames.at(iterator));
	}

	iterator++;

	return input;
}

int PalmReaderEnrollment::readUserId(){
	return stoi(imageNames.at(iterator).substr(0, 3));
}

bool PalmReaderEnrollment::hasNextImage(){
	if (iterator < imageNames.size()) {
		return true;
	} else{
		return false;
	}
}

PalmReaderEnrollment::~PalmReaderEnrollment()
{
}
