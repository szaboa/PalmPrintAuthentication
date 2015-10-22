#include "Treshold.h"

using namespace cv;

Treshold::Treshold(Mat inputImage) : inputImage(inputImage)
{
}

Mat Treshold::binaryTresholdWithRange(int min, int max){
	Mat tresholdedImage = inputImage;
	for (int i = 0; i< tresholdedImage.rows; i++){
		for (int j = 0; j< tresholdedImage.cols; j++){

			int pixelValue = tresholdedImage.at<uchar>(i, j);
			//check whether value is within range
			if ((pixelValue > min) && (pixelValue < max)) {
				tresholdedImage.at<uchar>(i, j) = 255;
			}
			else{
				tresholdedImage.at<uchar>(i, j) = 0;
			}
		}
	}

	return inputImage;
}

Treshold::~Treshold()
{
}
