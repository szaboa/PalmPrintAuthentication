#pragma once
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <module_DatabaseAdapter/DbAdapter.h>
#include <module_Matcher/IMatcher.h>
#include <module_FeatureExtraction/IFeature.h>
#include <vector>
#include <iostream>


using namespace cv;

/**
 * @brief The PrincipalLineMatcher class is an implementation of the IMatcher, uses Chamfer matching to match line features
 */
class PrincipalLineMatcher : public IMatcher
{
public:
    /**
     * @brief PrincipalLineMatcher
     */
	PrincipalLineMatcher();

    /**
     * @brief Matches the given feature to the stored feature using Chamfer matching
     * @param f Line-feature object
     * @return The best matching distance, matched user id
     */
    std::pair<double,int> doMatching(IFeature *f) override;

    /**
     * @brief Returns the matched feature
     * @return Line-feature object
     */
    IFeature* getMatchedFeature() override;

	~PrincipalLineMatcher();
private:

	DbAdapter *dbAdapter = nullptr;

    /**
     * @brief Transforms the line feature into distance representation, intensity of non-line pixels are the distance to the closest line-pixel
     * @param Image that contains the extracted lines
     * @return Distance transformed image
     */
	Mat doDistanceTransformation(Mat img);

    IFeature* matchedFeature = nullptr;

	
};

