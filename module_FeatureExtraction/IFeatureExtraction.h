#pragma once

#include <string>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <module_FeatureExtraction/IFeature.h>

class IFeatureExtraction
{
public:
	virtual IFeature* doFeatureExtraction(cv::Mat roi) = 0;

	virtual ~IFeatureExtraction(){};
};

