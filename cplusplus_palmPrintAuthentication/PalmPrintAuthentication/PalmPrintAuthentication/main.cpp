#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include "RoiExtraction.h"
#include "PrincipalLineExtraction.h"
#include "Logger.h"
#include "Thinning.h"
#include "FeatureEncoder.h"
#include <string>


using namespace cv;

int main(int argc, char *argv[]){

	const std::string TAG = "[main]";

	// reading sample palmprint image
	Mat image = imread("sample_images/database_img7.jpg"); 

	if (!image.data){
		Logger::log(TAG, "Error reading input image");
		return -1;
	}

	Logger::log(TAG, "Image loaded successfuly.");
	resize(image, image, Size(640, 480));
	Logger::log(TAG, "Image resized to 640x480.");
	RoiExtraction roiExtraction(image);
	
	PrincipalLineExtraction lineExtraction(roiExtraction.getSquareRoi());

	FeatureEncoder encoder;
	encoder.encodeAndSave(3, lineExtraction.getPrincipalLines(), lineExtraction.getLineComponents());
	//std::string chainCode = ChainCode::getChainCode(lineExtraction.getPrincipalLines(), lineExtraction.getLineComponents());

	return 0;
}