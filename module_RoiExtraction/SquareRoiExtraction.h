#pragma once
#include <module_RoiExtraction/IRoiExtraction.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <module_RoiExtraction/IRegionSegmentation.h>

using namespace cv;

// Struct for holding the keypoints (valley points between fingers)
struct Keypoints {
	Point centerPoint;
	Point keypoint1;
	Point keypoint2;
	Point keypoint3;
	bool success;
};

class SquareRoiExtraction:public IRoiExtraction
{
public:
	SquareRoiExtraction(IRegionSegmentation* regionSegmentation);

	void doExtraction(const cv::Mat &inputImg) override;

	cv::Mat getRoi() override;

	~SquareRoiExtraction();

private:
	IRegionSegmentation* regionSegmentation;
	/*Cropped image size from the center of the original image*/
	const int CENTER_SIZE = 20;

	/*Original image*/
	Mat inputImage, inputImageCopy;

	/*Cropped image from the center of the original image*/
	Mat centerOfImage;

	/*
	* Crops the center of the original image (using @CENTER_SIZE range)
	* and convert it to YCbCr color model
	*/
	Mat cropCenterOfInputImage();

	/* Apply region growing algorithm */
	Mat applyRegionGrowing();

	/* Get keypoints (valley points between fingers) from the segmented image */
	Keypoints findKeypoints(const Mat &segmentedImage);

	/* Calculate and draw the square ROI based on the keypoints */
	void calcAndDrawSquareRoi(const Keypoints &keypoints);

	Mat squareRoi;
};

