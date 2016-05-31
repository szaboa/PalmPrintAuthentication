#pragma once
#include <module_Matcher/IMatcher.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <module_DatabaseAdapter/DbAdapter.h>
#include <module_FeatureExtraction/IFeature.h>
#include <vector>

/**
 * @brief The TextureMatcher class is an implementation of the IMatcher, uses Hamming distance to match line features
 */
class TextureMatcher:public IMatcher
{
public:
    /**
     * @brief TextureMatcher
     */
	TextureMatcher();
	
    /**
     * @brief Matches the given feature to the stored feature using Hamming distance
     * @param f Texture-feature object
     * @return The best matching distance, matched user id
     */
    std::pair<double,int> doMatching(IFeature* f) override;

    /**
     * @brief Returns the matched feature
     * @return Texture-feature object
     */
    IFeature* getMatchedFeature() override;

	~TextureMatcher();

private:
	DbAdapter* dbAdapter = nullptr;

    /**
     * @brief Decodes the stored features
     * @param values Stored feature values
     * @return Decoded image
     */
	cv::Mat decode(std::vector<int> values);

    IFeature* matchedFeature = nullptr;
};

