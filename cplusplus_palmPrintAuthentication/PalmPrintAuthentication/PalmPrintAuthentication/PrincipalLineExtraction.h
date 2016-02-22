#pragma once


#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <vector>
#include <iostream>
#include <string>

using namespace cv;

/*
	This class extracts the principal lines from the ROI (output of the RoiExtraction phase)
	and stores it, using chain codes
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

	/* Defines the range of the first-order derivative's change detection*/
	int firstDerivChangeWidth = 4;

	/* Defines the second-order derivative's treshold value*/
	int secondDerivTresholdValue = 10;
	
	/* Defines the ROI's width and height in this phase*/
	static const int roiWidth  = 128;
	static const int roiHeight = 128;

	/* Normalizes the image  (using Histogram Equalization and a Low-pass Filter)*/
	void normalizeImage(Mat &img);

	/* Checks if the given coordinate is inside the image*/
	bool isInsideTheBoundary(int i, int j);

	/* Locates principal lines in a given direction*/
	Mat locatePrincipalLineInGivenDirection(const Mat &img, std::vector<std::vector<double>> H1, std::vector<std::vector<double>> H2, int degree);

	/* Locates the final principal lines (contains the merged lines) */
	void locatePrincipalLines();
};

