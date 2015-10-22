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

	/*Min and max color values in the first, second and thrid chanel */
	int minValueChanel1, minValueChanel2, minValueChanel3;
	int maxValueChanel1, maxValueChanel2, maxValueChanel3;

	/*Calculate and display the histogram*/
	void calculateHistogram();

public:
	DisplayHistogram(Mat img);

	int getMinValueChanel1();
	int getMaxValueChanel1();

	int getMinValueChanel2();
	int getMaxValueChanel2();

	int getMinValueChanel3();
	int getMaxValueChanel3();
	~DisplayHistogram();
};

