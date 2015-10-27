#pragma once

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <vector>
using namespace cv;

/*
* This class extracts the ROI of the given palm print image
*/

class RoiExtraction
{

private:
	/*Cropped image size from the center of the original image*/
	const int CENTER_SIZE = 20;

	/*Original image*/
	Mat inputImage;

	/*Cropped image from the center of the original image*/	
	Mat centerOfImage;
		
	/*
	* Crops the center of the original image (using @CENTER_SIZE range)
	* and convert it to YCbCr color model
	*/
	Mat cropCenterOfInputImage();

public:
	RoiExtraction(Mat image);
	~RoiExtraction();
};

