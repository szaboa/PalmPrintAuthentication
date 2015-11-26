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
#include "BoundaryTracking.h"
#include <fstream>

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

	
	// APPLY DILATION 
	output = MorphologicalOperation::dilate(output);
	namedWindow("After region growing and dilation", CV_WINDOW_AUTOSIZE);
	imshow("After region growing and dilation", output);
	 
	vector<Point> boundaryVector = BoundaryTracking::getBoundary(output); 

	//for (auto it = boundaryVector.begin(); it < boundaryVector.end() - 10; it += 10){
//		line(inputImage, Point(it->x + inputImage.cols / 2, it->y), Point((it + 10)->x + inputImage.cols / 2, (it + 10)->y), Scalar(0, 0, 255), 1);
	//}
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


	Point centerPoint(inputImage.cols / 2, (top.y + bottom.y) / 2);
	circle(inputImage, centerPoint, 2, CV_RGB(0, 255, 0), -1);

	vector<point_with_distance> distances;
	
	
	
	for (int i = 0; i < boundaryVector.size(); ++i){

		point_with_distance p;
		p.p.x = boundaryVector.at(i).x + inputImage.cols/2;
		p.p.y = boundaryVector.at(i).y;

		p.d = sqrt(pow(centerPoint.x - p.p.x, 2) + pow(centerPoint.y - p.p.y, 2));
		distances.push_back(p);
	}

	// mean filtering
	vector<point_with_distance> filtered_distances = distances;
	
	double sum = 0.0;
	double kernel_size = 5;

	for (int index = 2; index < distances.size()-2; index++){
		sum = distances.at(index - 2).d + distances.at(index - 1).d + distances.at(index).d + distances.at(index + 1).d + distances.at(index + 2).d;
		filtered_distances.at(index).d = sum / kernel_size;
		//cout << sum / kernel_size << endl;
	}

	int n = 5;
	vector<point_with_distance> minimums;
	for (int i = 50; i < filtered_distances.size() - 50; ++i){
		if (filtered_distances.at(i - n).d > filtered_distances.at(i).d && filtered_distances.at(i + n).d > filtered_distances.at(i).d)
		{
			point_with_distance temp;
			temp.p = filtered_distances.at(i).p;
			temp.d = filtered_distances.at(i).d;
			
			//circle(inputImage, filtered_distances.at(i).p, 2, CV_RGB(0, 255, 0), -1);
			minimums.push_back(temp);
		}
	}

	cout << "MIN size: " << minimums.size() << endl;

	vector<vector<point_with_distance>> min_clusters;
	vector<point_with_distance> temp_row;
	for (int i = 0; i < minimums.size() - 1; ++i){
		point_with_distance current, next;
		current = minimums.at(i);
		next = minimums.at(i + 1);
		float dist = sqrt(pow(current.p.x - next.p.x, 2) + pow(current.p.y - next.p.y, 2));
		if (dist > 10){
			min_clusters.push_back(temp_row);
			temp_row.clear();
		}
		else{
			temp_row.push_back(current);
		}
	}

	if (temp_row.size() != 0) min_clusters.push_back(temp_row);
	
	if (min_clusters.size() != 3) {
		cout << "Error : keypoints";
		return;
	}

	point_with_distance keypoint1, keypoint2, keypoint3;
	keypoint1 = *min_element(min_clusters[0].begin(), min_clusters[0].end(), [](const point_with_distance& x, const point_with_distance& y) {return x.d < y.d; });
	circle(inputImage, keypoint1.p, 2, CV_RGB(0, 255, 0), -1);

	keypoint2 = *min_element(min_clusters[1].begin(), min_clusters[1].end(), [](const point_with_distance& x, const point_with_distance& y) {return x.d < y.d; });
	circle(inputImage, keypoint2.p, 2, CV_RGB(0, 255, 0), -1);

	keypoint3 = *min_element(min_clusters[2].begin(), min_clusters[2].end(), [](const point_with_distance& x, const point_with_distance& y) {return x.d < y.d; });
	circle(inputImage, keypoint3.p, 2, CV_RGB(0, 255, 0), -1);

	line(inputImage, keypoint1.p, keypoint3.p, Scalar(0, 0, 255), 1);


	namedWindow("Original Image", CV_WINDOW_AUTOSIZE);
	imshow("Original Image", inputImage);


	///// Show in a window
	//namedWindow("Contours", CV_WINDOW_AUTOSIZE);
	//imshow("Contours", drawing);

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
