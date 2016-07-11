#pragma once
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

/**
 * @brief The IFeature class is an interface for representing a biometric feature
 */
class IFeature
{
public:
    /**
     * @brief Saves the feature into database with a given user id
     * @param userId The user id which belongs to this feature
     */
	virtual void save(int userId) = 0;

    /**
     * @brief Returns the image representation of the feature
     * @return Image that contains the feature
     */
    virtual cv::Mat getImageRepresentation() = 0;

    virtual void setUserId(int) = 0;
    virtual int getUserId() = 0;
    /**
     * @brief ~IFeature
     */
	virtual ~IFeature(){};
};

