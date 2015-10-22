#include "DisplayHistogram.h"
#include <vector>
#include <iostream>
#include <algorithm>

using namespace cv;
using namespace std;

DisplayHistogram::DisplayHistogram(Mat img) : img(img)
{
	calculateHistogram();
}

void DisplayHistogram::calculateHistogram(){
	// separate the image in 3 chanels
	vector<Mat> chanels;
	split(img, chanels);

	/// Establish the number of bins
	int histSize = 256;

	/// Set the ranges ( for B,G,R) )
	float range[] = { 0, 256 };
	const float* histRange = { range };

	bool uniform = true; bool accumulate = false;

	Mat histChanel1, histChanel2, histChanel3;

	/// Compute the histograms:
	calcHist(&chanels[0], 1, 0, Mat(), histChanel1, 1, &histSize, &histRange, uniform, accumulate);
	calcHist(&chanels[1], 1, 0, Mat(), histChanel2, 1, &histSize, &histRange, uniform, accumulate);
	calcHist(&chanels[2], 1, 0, Mat(), histChanel3, 1, &histSize, &histRange, uniform, accumulate);

	// Draw the histograms for Y, Cb and Cr
	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound((double)hist_w / histSize);

	Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));

	/// Normalize the result to [ 0, histImage.rows ]
	normalize(histChanel1, histChanel1, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(histChanel2, histChanel2, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(histChanel3, histChanel3, 0, histImage.rows, NORM_MINMAX, -1, Mat());

	/// Draw for each channel
	for (int i = 1; i < histSize; i++)
	{
		// draw histogram for chanel 1
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(histChanel1.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(histChanel1.at<float>(i))),
			Scalar(255, 255, 255), 2, 8, 0);
		// draw histogram for chanel 2
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(histChanel2.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(histChanel2.at<float>(i))),
			Scalar(0, 0, 255), 2, 8, 0);
		// draw histogram for chanel 3
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(histChanel3.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(histChanel3.at<float>(i))),
			Scalar(255, 0, 0), 2, 8, 0);
	}

	// getting min and max color value for Y, Cb and Cr components

	vector<int> colorValuesChanel1;
	vector<int> colorValuesChanel2; 
	vector<int> colorValuesChanel3; 

	for (int i = 1; i < histSize; ++i){
		//cout << "Histogram value at " << i << ": " << cb_hist.at<float>(i) << endl;
		if (histChanel1.at<float>(i) != 0){
			colorValuesChanel1.push_back(i);
		}

		if (histChanel2.at<float>(i) != 0){
			colorValuesChanel2.push_back(i);
		}

		if (histChanel3.at<float>(i) != 0){
			colorValuesChanel3.push_back(i);
		}
	}

	// getting min and max values

	auto it_minChanel1 = min_element(begin(colorValuesChanel1), end(colorValuesChanel1));
	auto it_maxChanel1 = max_element(begin(colorValuesChanel1), end(colorValuesChanel1));


	auto it_minChanel2 = min_element(begin(colorValuesChanel2), end(colorValuesChanel2));
	auto it_maxChanel2 = max_element(begin(colorValuesChanel2), end(colorValuesChanel2));

	auto it_minChanel3 = min_element(begin(colorValuesChanel3), end(colorValuesChanel3));
	auto it_maxChanel3 = max_element(begin(colorValuesChanel3), end(colorValuesChanel3));

	// storing min and max values

	minValueChanel1 = *it_minChanel1;
	maxValueChanel1 = *it_maxChanel1;

	minValueChanel2 = *it_minChanel2;
	maxValueChanel2 = *it_maxChanel2;

	minValueChanel3 = *it_minChanel3;
	maxValueChanel3 = *it_maxChanel3;

	cout << "Min value [Cr]: " << *it_minChanel2 << endl;
	cout << "Max value [Cr]: " << *it_maxChanel2 << endl;
	cout << "Min value [Cb]: " << *it_minChanel3 << endl;
	cout << "Max value [Cb]: " << *it_maxChanel3 << endl;

	/// Display
	namedWindow("calcHist", CV_WINDOW_AUTOSIZE);
	imshow("calcHist", histImage);
}

int DisplayHistogram::getMinValueChanel1(){
	return minValueChanel1;
}

int DisplayHistogram::getMaxValueChanel1(){
	return maxValueChanel1;
}

int DisplayHistogram::getMinValueChanel2(){
	return minValueChanel2;
}

int DisplayHistogram::getMaxValueChanel2(){
	return maxValueChanel2;
}

int DisplayHistogram::getMinValueChanel3(){
	return minValueChanel3;
}

int DisplayHistogram::getMaxValueChanel3(){
	return maxValueChanel3;
}

DisplayHistogram::~DisplayHistogram()
{
}
