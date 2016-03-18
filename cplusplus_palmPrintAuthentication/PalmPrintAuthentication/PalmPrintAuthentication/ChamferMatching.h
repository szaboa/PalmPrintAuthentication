#pragma once
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;

class ChamferMatching
{
public:
	ChamferMatching(Mat im);
	~ChamferMatching();

private:
	Mat searchImage;

	Mat doDistanceTransformation(Mat img);

	
};

