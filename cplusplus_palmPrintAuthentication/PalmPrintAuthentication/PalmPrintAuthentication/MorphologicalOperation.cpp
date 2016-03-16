#include "MorphologicalOperation.h"


using namespace std;
using namespace cv;

Mat MorphologicalOperation::dilate(const Mat &img, int erosionSize){
	Mat dst;

	Mat element = getStructuringElement(cv::MORPH_CROSS,
		cv::Size(2 * erosionSize + 1, 2 * erosionSize + 1),
		cv::Point(erosionSize, erosionSize));

	::dilate(img, dst, element);

	return dst;
}

Mat MorphologicalOperation::erode(const Mat &img){
	// TODO: implement later
	return img;
}