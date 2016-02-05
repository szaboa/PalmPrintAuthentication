#include "PrincipalLineExtraction.h"


using namespace std;
using namespace cv;

PrincipalLineExtraction::PrincipalLineExtraction(Mat roi) : roi(roi)
{
	// Convert to grayscale
	cvtColor(roi, this->roi, CV_BGR2GRAY);
	resize(this->roi, this->roi, Size(128, 128));
	
	// Normalize to 0-255
	normalizeImage(this->roi);

	// Make five copies of the normalized image
	Mat img0   = this->roi.clone();
	Mat img45  = this->roi.clone();
	Mat img90  = this->roi.clone();
	Mat img135 = this->roi.clone();

	
	// Use median filter on the first copy
	medianBlur(img0, img0, 3);

	// Define kernels for avarage filters (four directions)
	float kernel0_data[25] = { 0, 0, 0, 0, 0, 
							   0, 0, 0, 0, 0, 
							   1, 1, 1, 1, 1, 
							   0, 0, 0, 0, 0, 
							   0, 0, 0, 0, 0 };

	Mat kernel0 = cv::Mat(5, 5, CV_32F, kernel0_data);
	kernel0 = kernel0 / 5;

	float kernel45_data[25] = { 1, 0, 0, 0, 0,
								0, 1, 0, 0, 0,
								0, 0, 1, 0, 0,
								0, 0, 0, 1, 0,
								0, 0, 0, 0, 1 };

	Mat kernel45 = cv::Mat(5, 5, CV_32F, kernel45_data);
	kernel45 = kernel45 / 5;

	float kernel90_data[25] = { 0, 0, 1, 0, 0,
								0, 0, 1, 0, 0,
								0, 0, 1, 0, 0,
								0, 0, 1, 0, 0,
								0, 0, 1, 0, 0 };

	Mat kernel90 = cv::Mat(5, 5, CV_32F, kernel90_data);
	kernel90 = kernel90 / 5;

	float kernel135_data[25] = { 0, 0, 0, 0, 1,
								 0, 0, 0, 1, 0,
								 0, 0, 1, 0, 0,
								 0, 1, 0, 0, 0,
								 1, 0, 0, 0, 0 };

	Mat kernel135 = cv::Mat(5, 5, CV_32F, kernel135_data);
	kernel135 = kernel135 / 5;
	// Apply directional avarage filters

	filter2D(img0, img0, img0.depth(), kernel0, Point(-1, -1), 0, BORDER_DEFAULT);

	filter2D(img45,  img45,  img45.depth(),  kernel45,  Point(-1, -1), 0, BORDER_DEFAULT);
	filter2D(img90,  img90,  img90.depth(),  kernel90,  Point(-1, -1), 0, BORDER_DEFAULT);
	filter2D(img135, img135, img135.depth(), kernel135, Point(-1, -1), 0, BORDER_DEFAULT);

	


	// Apply black-hat transformation


	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));

	/// Apply the specified morphology operation
	morphologyEx(img0, img0, MORPH_BLACKHAT, element);
	
	

	namedWindow("Normalized Roi", CV_WINDOW_AUTOSIZE);
	imshow("Normalized Roi", img0);

	waitKey(0);

}

void PrincipalLineExtraction::normalizeImage(Mat &img){

	// getting min and max intensity values
	double minVal, maxVal;
	minMaxLoc(img, &minVal, &maxVal, NULL, NULL);

	// normalize every pixel
	for (int i = 0; i < img.rows; i++){
		for (int j = 0; j < img.cols; j++){
			img.at<uchar>(i, j) = (img.at<uchar>(i, j) - minVal) * (255 - 0) / (maxVal - minVal) + 0;
		}
	}
}

PrincipalLineExtraction::~PrincipalLineExtraction()
{
}
