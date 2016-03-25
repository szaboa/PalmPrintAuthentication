#include <module_RoiExtraction\OtsuTresholding.h>
#include <utility\MorphologicalOperation.h>
using namespace cv;

OtsuTresholding::OtsuTresholding()
{
}


Mat OtsuTresholding::doPreprocessing(const cv::Mat &inputImage){
	Mat y;

	cvtColor(inputImage, y, CV_RGB2YCrCb);
	

	namedWindow("Original Image", CV_WINDOW_AUTOSIZE);
	imshow("Original Image", y);
	waitKey(0);

	Rect leftSideRec(0, 0, inputImage.cols / 2 + 200, inputImage.rows);
	Mat leftSide = inputImage(leftSideRec);
	Mat grayscaleLeftSide(leftSide.size(), CV_8UC1);

	cvtColor(leftSide, grayscaleLeftSide, CV_RGB2GRAY);

	return grayscaleLeftSide;
}

Mat OtsuTresholding::doRegionSegmentation(const cv::Mat &preprocessedImage){
	Mat segmentedImage;

	threshold(preprocessedImage, segmentedImage, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

	return MorphologicalOperation::dilate(MorphologicalOperation::erode(segmentedImage, 2), 3);

//	return segmentedImage;
}


OtsuTresholding::~OtsuTresholding()
{
}
