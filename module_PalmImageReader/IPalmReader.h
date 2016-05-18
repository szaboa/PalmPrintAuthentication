#pragma once

#include <string>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class IPalmReader
{
public:
	virtual void init(std::string path) = 0;

	virtual cv::Mat readPalmImage() = 0;

	virtual int readUserId() = 0;

	virtual ~IPalmReader(){};
};

