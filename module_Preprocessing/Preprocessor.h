#pragma once
#include <module_Preprocessing/IPreprocessor.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

/**
 * @brief The Preprocessor class is an implementation of the IPreprocessor, uses skin-color modell to segment the skin pixels
 */
class Preprocessor : public IPreprocessor
{
public:
    /**
     * @brief Preprocessor
     */
    Preprocessor();

    /**
     * @brief Does preprocessing and hand segmentation on the given image, using skin-color modell
     * @param img Input image
     * @return Segmented image
     */
	cv::Mat doPreprocessing(cv::Mat img) override;

    ~Preprocessor();
};

