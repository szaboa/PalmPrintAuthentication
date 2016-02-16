#include "PrincipalLineExtraction.h"
#include <fstream>

using namespace std;
using namespace cv;

PrincipalLineExtraction::PrincipalLineExtraction(Mat roi) : roi(roi)
{
	// Convert to grayscale
	cvtColor(roi, this->roi, CV_BGR2GRAY);
	resize(this->roi, this->roi, Size(128, 128));

	//this->roi.at<uchar>(0, 10) = 0;
	namedWindow("Before normalization", CV_WINDOW_AUTOSIZE);
	imshow("Before normalization", this->roi);

	// locate principal lines in four directions, then merge them
	locatePrincipalLines(this->roi);

	waitKey(0);

}

void PrincipalLineExtraction::locatePrincipalLines(Mat &img){

	// defining horizontal line detecors

	// 1-D Gaussian function x first-order derivative of 1-D Gaussian function
	double H1[5][9] = {
		{ 0.0009, 0.0027, 0.0058, 0.0092, 0.0107, 0.0092, 0.0058, 0.0027, 0.0009 },
		{ 0.0065, 0.0191, 0.0412, 0.0655, 0.0764, 0.0655, 0.0412, 0.0191, 0.0065 },
		{ 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000 },
		{ -0.0009, -0.0027, -0.0058, -0.0092, -0.0107, -0.0092, -0.0058, -0.0027, -0.0009 },
		{ -0.0065, -0.0191, -0.0412, -0.0655, -0.0764, -0.0655, -0.0412, -0.0191, -0.0065 }
	};


	// 1-D Gaussian function x second-order derivative of 1-D Gaussian function
	double H2[5][9] = {
		{ 0.0156, 0.0211, 0.0309, 0.0416, 0.0464, 0.0416, 0.0309, 0.0211, 0.0156 },
		{ 0.0257, 0.0510, 0.0954, 0.1441, 0.1660, 0.1441, 0.0954, 0.0510, 0.0257 },
		{ -0.0298, -0.1125, -0.2582, -0.4178, -0.4896, -0.4178, -0.2582, -0.1125, -0.0298 },
		{ 0.0257, 0.0510, 0.0954, 0.1441, 0.1660, 0.1441, 0.0954, 0.0510, 0.0257 },
		{ 0.0156, 0.0211, 0.0309, 0.0416, 0.0464, 0.0416, 0.0309, 0.0211, 0.0156 }
	};

	// First derivative of the input image
	double I_der[128][128];

	// Second derivate of the input image
	double I_der2[128][128];


	// Initialize first- and second-order derivatives
	for (int i = 0; i < 128; ++i){
		for (int j = 0; j < 128; ++j){
			I_der[i][j]  = 0.0;
			I_der2[i][j] = 0.0;
		}
	}

	// Calculate first- and second-order derivatives
	for (int i = 2; i < img.rows-2; ++i){
		for (int j = 4; j < img.cols-4; ++j){

			for (int _i = -2; _i <= 2; ++_i){
				for (int _j = -4; _j <= +4; ++_j){
						I_der[i][j]  += static_cast<int>(img.at<uchar>(i +_i, j + _j)) * H1[_i+2][_j+4];
						I_der2[i][j] += static_cast<int>(img.at<uchar>(i +_i, j + _j)) * H2[_i+2][_j+4];
					}
			}
		}
	}

	//--- temp log
	ofstream f1, f2, f3;
	f1.open("original.txt");
	//f2.open("h1.txt");
	f2.open("der.txt");
	cout << "Original Image: " << endl;
	for (int i = 0; i < 15; ++i){
		for (int j = 0; j < 15; ++j){
			f1 << (int)img.at<uchar>(i, j) << " ";
		}
		f1 << endl;
	}

	int c = 1;
	for (int i = 0; i < 128; ++i){
		for (int j = 0; j < 128; ++j){
			f2 << I_der2[i][j] << "," << c << endl;
			c++;
		}
		
	}

	// Create binary image, this will contain the extracted principal lines
	Mat binaryImage(this->roi.size(), CV_8U);

	for (int i = 1; i < 126; ++i){
		for (int j = 3; j < 124; ++j){
			if (((I_der[i][j] * I_der[i][j - 1]) < 0 || I_der[i][j] == 0) && I_der2[i][j] > 6){
				binaryImage.at<uchar>(i, j) = 255;
			}
			else{
				binaryImage.at<uchar>(i, j) = 0;
			}
		}
	}
	
	f1.close();
	f2.close();
	namedWindow("Lines", CV_WINDOW_AUTOSIZE);
	imshow("Lines", binaryImage);

}

PrincipalLineExtraction::~PrincipalLineExtraction()
{
}
