#include "RoiExtraction.h"
#include "Histogram.h"
#include "PixelValueTreshold.h"
#include "RegionSegmentation.h"
#include <iostream>
#include <vector>
#include <queue>
#include <chrono>
#include <ctime>

using namespace std;
using namespace cv;

RoiExtraction::RoiExtraction(Mat inputImage) : inputImage(inputImage)
{
	char* sourceWindow = "Source";
	namedWindow(sourceWindow, CV_WINDOW_AUTOSIZE);
	imshow(sourceWindow, inputImage);
	
	// crop center of input image
	centerOfImage = cropCenterOfInputImage();
	
	// get an instance from Histogram
	Histogram::getInstance()->init(centerOfImage);

	//split input image to YCbCr chanels
	Mat inputImageYCbCr;
	cvtColor(inputImage, inputImageYCbCr, COLOR_BGR2YCrCb);
	vector<Mat> ycbcr_planes;
	split(inputImageYCbCr, ycbcr_planes);

	namedWindow("Before Treshold image", CV_WINDOW_AUTOSIZE);
	imshow("Before Treshold image", inputImageYCbCr);
	
	cout << "Start region growing algorithm.." << endl;
	chrono::time_point<chrono::system_clock> start, end;
	start = chrono::system_clock::now();
	//Mat output = RegionSegmentation::applyRegionGrowingAlgorithm(inputImageYCbCr);
	Mat output = RegionSegmentation::applyRegionGrowingAlgorithm(inputImageYCbCr);
	end = chrono::system_clock::now();
	chrono::duration<double> elapsed_seconds = end - start;

	cout << "Finished in: " << elapsed_seconds.count() << " seconds";
	//Mat output;
	//inRange(inputImageYCbCr, Scalar(0, Histogram::getMinValueChanel2() - 5, Histogram::getMinValueChanel3() - 5), Scalar(255, Histogram::getMaxValueChanel2() + 5, Histogram::getMaxValueChanel3() + 5), output);

	//namedWindow("Tresholded image", CV_WINDOW_AUTOSIZE);
	//imshow("Tresholded image", output);

	
	namedWindow("After region growing", CV_WINDOW_AUTOSIZE);
	imshow("After region growing", output);
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
