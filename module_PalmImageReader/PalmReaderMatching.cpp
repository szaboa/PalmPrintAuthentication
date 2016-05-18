#include <module_PalmImageReader/PalmReaderMatching.h>
#include <utility/PPAException.h>

using namespace std;

PalmReaderMatching::PalmReaderMatching()
{
}

void PalmReaderMatching::init(std::string path) {
	this->path = path;
}

cv::Mat PalmReaderMatching::readPalmImage() {
	
	cv::Mat input = cv::imread(path);
	
	if (!input.data) throw PPAException("Error during reading image");

	return input;
}

int PalmReaderMatching::readUserId(){
	return 0;
}


PalmReaderMatching::~PalmReaderMatching()
{
}
