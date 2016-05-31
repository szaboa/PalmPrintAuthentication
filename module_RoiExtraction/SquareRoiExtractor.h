#pragma once
#include <module_RoiExtraction/IRoiExtractor.h>
#include <module_Preprocessing/IPreprocessor.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

/**
 * @brief The Keypoints structure (valley points between fingers and a reference point)
 */
struct Keypoints {
    /**
     * @brief centerPoint Middle point between the top and bottom boundary of the hand (from left-most column)
     */
	Point centerPoint;
    /**
     * @brief keypoint1 Valley-point between the index and middle finger
     */
	Point keypoint1;
    /**
     * @brief keypoint2 Valley-point between the middle and third finger
     */
	Point keypoint2;
    /**
     * @brief keypoint3 Valley-point between the third and little finger
     */
	Point keypoint3;
    /**
     * @brief success Indicates if the keypoints were successfuly detected or not
     */
	bool success;
};

/**
 * @brief The SquareRoiExtractor class is an implementation of the IRoiExtractor, extracts square-shaped area from the segmented image
 */
class SquareRoiExtractor:public IRoiExtractor
{
public:
    /**
     * @brief SquareRoiExtractor
     * @param image Original image
     * @param preprocessor Preprocessor implementation
     */
    SquareRoiExtractor(Mat image, IPreprocessor* preprocessor);

    /**
     * @brief Extracts the square-shaped palm area (ROI)
     * @return The extracted ROI
     */
    Mat doExtraction() override;


    ~SquareRoiExtractor();

private:
    /**
     * @brief Preprocessor implementation
     */
    IPreprocessor* preprocessor;

    /**
     * @brief Original image
     */
	Mat inputImage, inputImageCopy;

    /**
     * @brief Localizes the keypoints from the segmented image
     * @param segmentedImage Segmented image
     * @return Localized keypoints
     */
	Keypoints findKeypoints(const Mat &segmentedImage);

    /**
     * @brief Calculates the square ROI based on the given keypoints
     * @param keypoints Localized keypoints
     * @return The calculated ROI
     */
    Mat calcSquareRoi(const Keypoints &keypoints);
};

