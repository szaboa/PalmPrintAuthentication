#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include "RoiExtraction.h"

using namespace cv;



int main(int argc, char *argv[]){

	// reading sample palmprint image
	Mat image = imread("sample_images/sample_picture_2015-10-21_12-22-29.jpg");

	RoiExtraction roiExtraction(image);

	return 0;

}