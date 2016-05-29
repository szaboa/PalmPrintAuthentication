#pragma once
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class IFeature
{
public:
	virtual void save(int userId) = 0;
    virtual cv::Mat getFeature() = 0;
	virtual ~IFeature(){};
};

