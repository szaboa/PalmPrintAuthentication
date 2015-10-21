#include "RoiExtraction.h"

using namespace cv;

RoiExtraction::RoiExtraction(Mat inputImage) : inputImage(inputImage)
{
	char* sourceWindow = "Source";
	namedWindow(sourceWindow, CV_WINDOW_AUTOSIZE);
	imshow(sourceWindow, inputImage);

	getCenterOfInputImage();
}

void RoiExtraction::getCenterOfInputImage(){
	Rect roi = Rect(0,0, CENTER_SIZE, CENTER_SIZE);
	centerOfImage = inputImage(roi);

	char* centerOfImageWindow = "Center of image";
	namedWindow(centerOfImageWindow, CV_WINDOW_AUTOSIZE);
	imshow(centerOfImageWindow, centerOfImage);
	waitKey(0);
}


RoiExtraction::~RoiExtraction()
{
}
