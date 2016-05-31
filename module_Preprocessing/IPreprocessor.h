#pragma once

#include <string>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

/**
 * @brief The IPreprocessor class is an interface for doing preprocessing on an image
 */
class IPreprocessor
{
public:

    /**
     * @brief Does preprocessing and hand segmentation on the given image
     * @param img Input image
     * @return Segmented image (white pixels representing the segmented hand)
     */
	virtual cv::Mat doPreprocessing(cv::Mat img) = 0;

    /**
     * @brief ~IPreprocessor
     */
    virtual ~IPreprocessor(){};
};

