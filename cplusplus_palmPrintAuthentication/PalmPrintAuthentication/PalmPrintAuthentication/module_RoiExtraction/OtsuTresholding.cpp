#include <module_RoiExtraction\OtsuTresholding.h>
#include <utility\MorphologicalOperation.h>
#include <boost\numeric\ublas\matrix.hpp>
#include <boost\numeric\ublas\io.hpp>
#include <math.h>
#include <iostream>
#include <vector>
#include<omp.h>

using namespace cv;
using namespace boost::numeric::ublas;
using namespace std;

OtsuTresholding::OtsuTresholding()
{
}



Mat OtsuTresholding::doPreprocessing(const cv::Mat &inputImage){

	
	Mat original = inputImage.clone();
	Mat ycbcr;
	cvtColor(original, ycbcr, CV_RGB2YCrCb);

	
	return ycbcr;
}

Mat OtsuTresholding::doRegionSegmentation(const cv::Mat &preprocessedImage){

	


	double c11 = 0.0479;
	double c12 = 0.0259;
	double c21 = 0.0259;
	double c22 = 0.0212;

	double k1 = 0.0;
	double k2 = 0.0;
	double x1, x2;
	double m1 = 113.9454;
	double m2 = 157.5052;
	double f1, f2;



	Mat thresholded(preprocessedImage.size(), CV_8UC1);
	thresholded.setTo(Scalar(0));

	double p = 0;

	for (int i = 0; i < preprocessedImage.rows; ++i){
		for (int j = 0; j < preprocessedImage.cols; ++j){

			x1 = (int)preprocessedImage.at<Vec3b>(i, j)[1];
			x2 = (int)preprocessedImage.at<Vec3b>(i, j)[2];

				f1 = -0.5*(x1 - m1);
				f2 = -0.5*(x2 - m2);
				k1 = f1*c11 + f2*c21;
				k2 = f1*c12 + f2*c22;

				p = exp(k1*(x1 - m1) + k2*(x2 - m2));
		

				if (p > 0.15){
					thresholded.at<uchar>(i, j) = 255;
				}
			

		}
	}

	bool TEST = false;

	if (TEST){
		//Mat final = MorphologicalOperation::closing(thresholded, 2);
		//final = MorphologicalOperation::erode(final, 3);


		namedWindow("pls", WINDOW_AUTOSIZE);
		imshow("pls", thresholded);
		waitKey(0);
	}

	/*
	
	*/
	return thresholded;

}


OtsuTresholding::~OtsuTresholding()
{
}
