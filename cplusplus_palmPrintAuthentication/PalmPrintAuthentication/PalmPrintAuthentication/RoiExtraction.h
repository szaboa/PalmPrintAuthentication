#pragma once

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
class RoiExtraction
{

private:
	const int CENTER_SIZE = 20;
	Mat inputImage;
	Mat centerOfImage;
public:
	RoiExtraction(Mat image);

	void getCenterOfInputImage();
	~RoiExtraction();
};

