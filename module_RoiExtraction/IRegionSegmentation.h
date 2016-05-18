#pragma once

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class IRegionSegmentation
{
public:
	
	virtual cv::Mat doPreprocessing(const cv::Mat &inputImage) = 0;

	virtual cv::Mat doRegionSegmentation(const cv::Mat &inputImg) = 0;

	virtual ~IRegionSegmentation(){};
};

