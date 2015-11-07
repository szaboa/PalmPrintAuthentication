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
	cout << "cols: " << inputImage.cols << endl;
	//Rect rightSideRoi(0, inputImage.cols / 2, inputImage.cols/2, inputImage.rows);
	Rect rightSideRoi(inputImage.cols / 2, 0, inputImage.cols / 2, inputImage.rows);
	Mat rightSide = inputImageYCbCr(rightSideRoi);
	
	cout << "Start region growing algorithm.." << endl;
	chrono::time_point<chrono::system_clock> start, end;
	start = chrono::system_clock::now();

	Mat output = RegionSegmentation::applyRegionGrowingAlgorithm(rightSide);
	end = chrono::system_clock::now();
	chrono::duration<double> elapsed_seconds = end - start;

	cout << "Finished in: " << elapsed_seconds.count() << " seconds";

	// EXPERIMENTING WITH DIFFERENT APPROACHES - THIS WILL BE ARRANGED LATER
	// eroding
	Mat dst;
	int erosion_size = 2;
	Mat element = getStructuringElement(cv::MORPH_CROSS,
		cv::Size(2 * erosion_size + 1, 2 * erosion_size + 1),
		cv::Point(erosion_size, erosion_size));

	dilate(output, dst, element);
	
	namedWindow("After region growing", CV_WINDOW_AUTOSIZE);
	imshow("After region growing", output);

	imwrite("beforeErosion.jpg", output);
	namedWindow("After erosion", CV_WINDOW_AUTOSIZE);
	imshow("After erosion", dst);


	// find contours

	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	RNG rng(12345);
	/// Detect edges using canny
	Canny(dst, canny_output, 100, 100 * 2, 3);
	/// Find contours
	findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	/// Draw contours
	Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
	for (int i = 0; i< contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
	}

	/// Show in a window
	namedWindow("Contours", CV_WINDOW_AUTOSIZE);
	imshow("Contours", drawing);

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
