#pragma once
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

class IMatcher
{
public:
	virtual int doMatching(cv::Mat img) = 0;

	virtual ~IMatcher(){};
};

