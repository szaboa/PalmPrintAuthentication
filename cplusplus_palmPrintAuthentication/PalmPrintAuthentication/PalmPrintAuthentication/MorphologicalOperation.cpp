#include "MorphologicalOperation.h"


using namespace std;
using namespace cv;

Mat MorphologicalOperation::dilate(const Mat &img){
	Mat dst;
	int erosion_size = 2;
	Mat element = getStructuringElement(cv::MORPH_CROSS,
		cv::Size(2 * erosion_size + 1, 2 * erosion_size + 1),
		cv::Point(erosion_size, erosion_size));

	::dilate(img, dst, element);

	return dst;
}

Mat MorphologicalOperation::erode(const Mat &img){
	// TODO: implement later
	return img;
}