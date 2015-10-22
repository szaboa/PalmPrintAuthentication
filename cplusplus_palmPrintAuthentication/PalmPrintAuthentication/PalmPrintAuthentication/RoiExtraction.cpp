#include "RoiExtraction.h"
#include "DisplayHistogram.h"
#include <vector>

using namespace std;
using namespace cv;

RoiExtraction::RoiExtraction(Mat inputImage) : inputImage(inputImage)
{
	char* sourceWindow = "Source";
	namedWindow(sourceWindow, CV_WINDOW_AUTOSIZE);
	imshow(sourceWindow, inputImage);

	centerOfImage = cropCenterOfInputImage();
	
	
	DisplayHistogram histogramDisplay(centerOfImage);

	//split input image to YCbCr chanels
	Mat inputImageYCbCr;
	cvtColor(inputImage, inputImageYCbCr, COLOR_BGR2YCrCb);
	vector<Mat> ycbcr_planes;
	split(inputImageYCbCr, ycbcr_planes);

	Mat output;
	inRange(inputImageYCbCr, Scalar(0, histogramDisplay.getMaxValueChanel2(), histogramDisplay.getMinValueChanel3()),
		Scalar(255, histogramDisplay.getMaxValueChanel2(), histogramDisplay.getMaxValueChanel3()), output);

	namedWindow("Tresholded image", CV_WINDOW_AUTOSIZE);
	imshow("Tresholded image", output);

	waitKey(0);

}


Mat RoiExtraction::cropCenterOfInputImage(){
	Size inputImageSize = inputImage.size();

	Rect roi = Rect((inputImageSize.width) / 2 - CENTER_SIZE / 2, (inputImageSize.height) / 2 - CENTER_SIZE / 2, CENTER_SIZE, CENTER_SIZE);
	Mat croppedImage = inputImage(roi);

	Mat croppedImageYCbCr;
	cvtColor(croppedImage, croppedImageYCbCr, CV_BGR2YCrCb);

	char* croppedImageWindow = "Cropped Image";
	namedWindow(croppedImageWindow, CV_WINDOW_AUTOSIZE);
	imshow(croppedImageWindow, croppedImageYCbCr);

	return croppedImageYCbCr;
}


RoiExtraction::~RoiExtraction()
{
}
