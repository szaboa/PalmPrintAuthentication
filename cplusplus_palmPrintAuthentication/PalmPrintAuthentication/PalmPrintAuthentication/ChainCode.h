#pragma once

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <string>

using namespace cv;

class ChainCode
{
public:

	static std::string getChainCode(const Mat &img, std::vector<std::vector<Point>> lineComponents);

private:
	/*private constructor */
	ChainCode(){	};
	/*private copy constructor*/
	ChainCode(ChainCode const&){};
	/*private assignment operator*/
	ChainCode& operator = (ChainCode const&){};
};

