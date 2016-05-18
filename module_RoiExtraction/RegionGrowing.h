#pragma once
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include <utility/Histogram.h>
#include <module_RoiExtraction/IRegionSegmentation.h>

class RegionGrowing:public IRegionSegmentation
{
public:
	RegionGrowing();
	cv::Mat doPreprocessing(const cv::Mat &inputImage) override;
	cv::Mat doRegionSegmentation(const cv::Mat &preprocessedImage) override;
	
	~RegionGrowing();

private:
	static const int CENTER_SIZE = 20;

	std::vector<cv::Point> getNeighbourPoints(const cv::Point &point);
	bool inRange(cv::Scalar value, cv::Scalar min, cv::Scalar max);
	cv::Mat cropCenterOfImage(const cv::Mat &inputImage, int centerSize);
	
	cv::Point seedPoint;
	Histogram* histogram;
	
};

