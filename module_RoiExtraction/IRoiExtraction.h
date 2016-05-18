#pragma once

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class IRoiExtraction
{
public:
	virtual void doExtraction(const cv::Mat &inputImg) = 0;

	virtual cv::Mat getRoi() = 0;

	virtual ~IRoiExtraction(){};
};

