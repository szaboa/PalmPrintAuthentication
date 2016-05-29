#pragma once
#include <module_Matcher/IMatcher.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <module_DatabaseAdapter/DbAdapter.h>
#include <module_FeatureExtraction/IFeature.h>
#include <vector>

class TextureMatcher:public IMatcher
{
public:
	TextureMatcher();
	
    std::pair<double,int> doMatching(IFeature* f) override;
    IFeature* getMatchedFeature() override;

	~TextureMatcher();

private:
	DbAdapter* dbAdapter = nullptr;

	cv::Mat decode(std::vector<int> values);

    IFeature* matchedFeature = nullptr;
};

