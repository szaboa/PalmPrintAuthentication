#pragma once


#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;

/*
* This class calculates and displays the histogram of the given image
*/
class DisplayHistogram
{

private:
	/*Input image, the histogram will be calculated based on this image*/
	Mat img;

	/*Calculate and display the histogram*/
	void calculateHistogram();

public:
	DisplayHistogram(Mat img);
	~DisplayHistogram();
};

