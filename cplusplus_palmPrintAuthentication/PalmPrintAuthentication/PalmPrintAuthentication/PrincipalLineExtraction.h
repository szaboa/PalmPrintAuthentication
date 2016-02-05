#pragma once


#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <vector>
#include <iostream>
#include <string>

using namespace cv;

/*
	This class extracts the principal lines from the ROI
*/
class PrincipalLineExtraction
{
public:

	/* Converts the given ROI to gray scale image and change its resolution to 128x128 */
	PrincipalLineExtraction(Mat roi);
	~PrincipalLineExtraction();

private:
	Mat roi;

	/*Linear normalization*/
	void normalizeImage(Mat &img);
};

