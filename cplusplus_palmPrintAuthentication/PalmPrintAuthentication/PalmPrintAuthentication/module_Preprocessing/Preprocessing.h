#pragma once
#include <module_Preprocessing\IPreprocessing.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class Preprocessing : public IPreprocessing
{
public:
	Preprocessing();
	cv::Mat doPreprocessing(cv::Mat img) override;
	~Preprocessing();
};

