#pragma once

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

/**
 * @brief The IRoiExtractor class is an interface for doing ROI extraction from a segmented image
 */
class IRoiExtractor
{
public:
    /**
     * @brief Doing ROI extraction
     * @return The extracted ROI
     */
    virtual cv::Mat doExtraction() = 0;

    /**
     * @brief ~IRoiExtractor
     */
    virtual ~IRoiExtractor(){};
};

