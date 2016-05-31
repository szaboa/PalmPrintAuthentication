#pragma once

#include <string>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <module_FeatureExtraction/IFeature.h>

/**
 * @brief The IFeatureExtractor class is an interferace, designed to do feature extraction
 */
class IFeatureExtractor
{
public:
    /**
     * @brief Extracts the feature, based on the given ROI
     * @param The extracted ROI
     * @return IFeature object
     */
	virtual IFeature* doFeatureExtraction(cv::Mat roi) = 0;

    /**
     * @brief ~IFeatureExtractor
     */
    virtual ~IFeatureExtractor(){};
};

