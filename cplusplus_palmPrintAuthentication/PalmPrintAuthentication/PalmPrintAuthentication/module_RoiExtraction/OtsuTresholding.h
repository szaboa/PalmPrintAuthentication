#pragma once
#include <module_RoiExtraction\IRegionSegmentation.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class OtsuTresholding : public IRegionSegmentation
{
public:
	OtsuTresholding();

	cv::Mat doPreprocessing(const cv::Mat &inputImage) override;
	cv::Mat doRegionSegmentation(const cv::Mat &preprocessedImage) override;

	~OtsuTresholding();
};

