#pragma once

#include <string>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class IPreprocessing
{
public:

	virtual cv::Mat doPreprocessing(cv::Mat img) = 0;

	virtual ~IPreprocessing(){};
};

