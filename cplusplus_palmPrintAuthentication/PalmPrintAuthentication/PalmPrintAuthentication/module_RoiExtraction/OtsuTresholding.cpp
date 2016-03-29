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


	matrix<double> invC(2, 2);
	invC(0, 0) = 0.0479;
	invC(0, 1) = 0.0259;
	invC(1, 0) = 0.0259;
	invC(1, 1) = 0.0212;

	matrix<double> m(2, 1);
	m(0, 0) = 113.9454;
	m(1, 0) = 157.5052;

	matrix<double> x(2, 1);

	Mat thresholded(preprocessedImage.size(), CV_8UC1);
	thresholded.setTo(Scalar(0));

	double p = 0;

	for (int i = 0; i < preprocessedImage.rows; ++i){
		for (int j = 0; j < preprocessedImage.cols; ++j){

			x(0, 0) = (int)preprocessedImage.at<Vec3b>(i, j)[1];
			x(1, 0) = (int)preprocessedImage.at<Vec3b>(i, j)[2];

			//cout << x(0, 0) << " " << x(1, 0) << endl;

			matrix<double> transpose = trans(x - m);
			matrix<double> scalarTranspose = -0.5 * transpose;
			matrix<double> firstP = prod(scalarTranspose, invC);
			matrix<double> secondP = prod(firstP, (x - m));

			p = exp(secondP(0, 0));

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
