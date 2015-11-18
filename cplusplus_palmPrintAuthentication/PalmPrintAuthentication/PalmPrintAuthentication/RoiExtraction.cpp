#include "RoiExtraction.h"
#include "Histogram.h"
#include "PixelValueTreshold.h"
#include "RegionSegmentation.h"
#include "MorphologicalOperation.h"
#include <iostream>
#include <vector>
#include <queue>
#include <chrono>
#include <ctime>
#include <math.h>
#include <algorithm>

using namespace std;
using namespace cv;

struct point_with_distance{
	Point p;
	double d;
};
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


	// CROP THE RIGHT SIDE OF THE IMAGE
	Rect rightSideRoi(inputImage.cols / 2, 0, inputImage.cols / 2, inputImage.rows);
	Mat rightSide = inputImageYCbCr(rightSideRoi);
	
	cout << "Start region growing algorithm.." << endl;
	chrono::time_point<chrono::system_clock> start, end;
	start = chrono::system_clock::now();


	// APPLY REGION GROWING ALGORITHM
	Mat output = RegionSegmentation::applyRegionGrowingAlgorithm(rightSide);
	end = chrono::system_clock::now();
	chrono::duration<double> elapsed_seconds = end - start;

	cout << "Finished in: " << elapsed_seconds.count() << " seconds";

	// EXPERIMENTING WITH DIFFERENT APPROACHES - THIS WILL BE ARRANGED LATER
	
	
	// APPLY DILATION 
	output = MorphologicalOperation::dilate(output);
	namedWindow("After region growing and dilation", CV_WINDOW_AUTOSIZE);
	imshow("After region growing and dilation", output);

	// FIND CONTOURS BY APPLYING CANNY 

	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	RNG rng(12345);
	/// Detect edges using canny
	Canny(output, canny_output, 100, 100 * 2, 3);
	/// Find contours
	findContours(canny_output, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point(0, 0));

	/// Draw contours
	Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
	for (int i = 0; i< contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
	}
	// get first top / bottom non-zero point
	Point top, bottom;

	bool foundUpper = false;
	for (int i = 0; i < output.rows; ++i){
		if (foundUpper){
			if (output.at<uchar>(i, 0) == 0){
				top.x = 0;
				top.y = i;
				break;
			}
		}
		else{
			if (output.at<uchar>(i, 0) == 255){
				foundUpper = true;
				bottom.x = 0;
				bottom.y = i;
			}
		}
		
	}

	cout << endl << top.x << " " << top.y << endl << bottom.x << " " << bottom.y << endl;
	Point centerPoint(inputImage.cols / 2, (top.y + bottom.y) / 2);
	circle(inputImage, centerPoint, 2, CV_RGB(0, 255, 0), -1);

	vector<point_with_distance> distances;

	
	for (int i = 0; i < contours.size(); ++i){
		for (int j = 0; j < contours[i].size(); ++j){
			//cout << contours[i][j].x << " " << contours[i][j].y << endl;
			point_with_distance p;
			p.p.x = contours[i][j].x + inputImage.cols/2;
			p.p.y = contours[i][j].y;
			p.d = sqrt(pow(centerPoint.x - p.p.x, 2) + pow(centerPoint.y - p.p.y, 2));
			distances.push_back(p);
		}
		
	}
	
	//sort(distances.begin(), distances.end(), [](const point_with_distance &a, const point_with_distance &b){ return a.d < b.d;  });

	

	auto it = distances.begin();
	for (auto it = distances.begin() + 1; it < distances.end() - 1; ++it){
		if (it->d < (it - 1)->d && it->d < (it + 1)->d){
			circle(inputImage, it->p, 2, CV_RGB(0, 255, 0), -1);
		}
	}

	
	Mat distanceGraph(Size(distances.size(), 400),CV_8U,Scalar(0,0,0));
	namedWindow("Distance Graph", CV_WINDOW_AUTOSIZE);
	cout << "Number of contours: " << contours.size() << endl;
	for (int i = 0;i<distances.size();++i){
		circle(distanceGraph, Point(i,distances.at(i).d), 2, CV_RGB(255, 255, 255), -1);
		//circle(distanceGraph, Point(distances.at(i).p), 2, CV_RGB(255, 255, 255), -1);
		//
		//waitKey(33);
	}
	imshow("Distance Graph", distanceGraph);
	
	

	namedWindow("Original Image", CV_WINDOW_AUTOSIZE);
	imshow("Original Image", inputImage);


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
