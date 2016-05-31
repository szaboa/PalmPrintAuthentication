#include <module_PalmImageReader/MultPalmReader.h>
#include <boost/filesystem.hpp>
#include <utility/PPAException.h>
#include <string>

using namespace std;
using namespace boost::filesystem;

MultPalmReader::MultPalmReader(){}

void MultPalmReader::init(std::string dirPath) {
	this->dirPath = dirPath;
	path path(dirPath);

    // Get filenames from the given directory
	for (auto i = directory_iterator(path); i != directory_iterator(); ++i){
		imageNames.push_back(i->path().filename().string());
	}
}

cv::Mat MultPalmReader::readPalmImage() {
	if (!hasNextImage()) throw PPAException("Error during reading images");
	
	cv::Mat input = cv::imread(dirPath + "/" + imageNames.at(iterator));
	
	if (!input.data) {
		throw PPAException("Error during reading images");
    }

	iterator++;

	return input;
}

int MultPalmReader::getNumberOfImages(){
    return imageNames.size();
}

int MultPalmReader::readUserId(){
    // First three digit of the filename represents the user id
	return stoi(imageNames.at(iterator).substr(0, 3));
}

bool MultPalmReader::hasNextImage(){
	if (iterator < imageNames.size()) {
		return true;
	} else{
		return false;
	}
}

MultPalmReader::~MultPalmReader()
{
}
