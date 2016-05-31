#pragma once
#include <module_FeatureExtraction/IFeature.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

/**
 * @brief The IMatcher class is an interface, designed to do 1:N matching
 */
class IMatcher
{
public:
    /**
     * @brief Matches the given feature to the stored feature
     * @param f IFeature implementation
     * @return The best matching distance, matched user id
     */
    virtual std::pair<double,int> doMatching(IFeature* f) = 0;

    /**
     * @brief Returns the matched feature
     * @return IFeature implementation
     */
    virtual IFeature* getMatchedFeature()=0;

    /**
     * @brief ~IMatcher
     */
	virtual ~IMatcher(){};
};

