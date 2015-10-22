#pragma once
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
/*This class contains thresholding methods*/
class Treshold
{

private:
	Mat inputImage;
public:
	Treshold(Mat inputImage);

	/*Binary treshold with a given range (@min, @max)*/
	Mat binaryTresholdWithRange(int min, int max);

	~Treshold();
};

