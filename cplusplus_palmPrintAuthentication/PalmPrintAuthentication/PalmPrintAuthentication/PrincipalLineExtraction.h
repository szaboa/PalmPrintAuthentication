#pragma once


#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <vector>
#include <iostream>
#include <string>

using namespace cv;

/*
	This class extracts the principal lines from the ROI (output of the RoiExtraction phase)
	and stores it using chain codes
*/
class PrincipalLineExtraction
{
public:

	/* Convert the given ROI to gray scale image and change its resolution to 128x128 */
	PrincipalLineExtraction(Mat roi);
	~PrincipalLineExtraction();

private:

	/* Input image, this is the output of the RoiExtraction phase*/
	Mat roi;

	/* Locate principal lines in four directions (0, 45, 90, 135), then merge them, 
	  and apply morphological operations to correct these lines*/
	void locatePrincipalLines(Mat &img);
};

