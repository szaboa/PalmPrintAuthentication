#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include "RoiExtraction.h"

using namespace cv;



int main(int argc, char *argv[]){

	// reading sample palmprint image
	Mat image = imread("sample_images/20151112_173809.jpg");
	resize(image, image, Size(640, 480));

	RoiExtraction roiExtraction(image);

	return 0;

}