#include <module_Preprocessing/Preprocessing.h>

using namespace cv;

Preprocessing::Preprocessing()
{
}

Mat Preprocessing::doPreprocessing(cv::Mat img){
	resize(img, img, Size(640, 480));

	return img;
}


Preprocessing::~Preprocessing()
{
}
