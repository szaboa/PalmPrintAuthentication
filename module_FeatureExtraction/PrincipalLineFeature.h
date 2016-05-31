#pragma once
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <module_DatabaseAdapter/DbAdapter.h>
#include <module_FeatureExtraction/IFeature.h>

/**
 * @brief The PrincipalLineFeature class is an implementation of the IFeature, represents a particular, line-based biometric feature
 */
class PrincipalLineFeature : public IFeature
{
public:
    /**
     * @brief Constructor
     * @param img Extracted feature image
     */
	PrincipalLineFeature(cv::Mat img);

    /**
     * @brief Saves the feature into database
     * @param userId The feature belongs to this user id
     */
	void save(int userId) override;

    /**
     * @brief Returns the image representation of the feature
     * @return Image that contains the feature
     */
    cv::Mat getImageRepresentation() override;

	~PrincipalLineFeature();

private:
	cv::Mat img;

	DbAdapter *dbAdapter = nullptr;
};

