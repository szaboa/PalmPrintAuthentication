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

	

	namedWindow("Normalized Roi", CV_WINDOW_AUTOSIZE);
	imshow("Normalized Roi", this->roi);

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
