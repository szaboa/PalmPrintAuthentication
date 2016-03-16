#pragma once
#include "DbAdapter.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <vector>

class FeatureEncoder
{

private:
	DbAdapter *dbAdapter;
public:
	FeatureEncoder();

	void encodeAndSave(int userId, const cv::Mat &img, std::vector<std::vector<cv::Point>> lineComponents);

	~FeatureEncoder();
};

