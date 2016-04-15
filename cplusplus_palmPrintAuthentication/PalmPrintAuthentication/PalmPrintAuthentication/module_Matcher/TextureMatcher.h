#pragma once
#include <module_Matcher\IMatcher.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <module_DatabaseAdapter\DbAdapter.h>
#include <module_FeatureExtraction\IFeature.h>
#include <vector>

class TextureMatcher:public IMatcher
{
public:
	TextureMatcher();
	
	int doMatching(IFeature* f) override;
	
	~TextureMatcher();

private:
	DbAdapter* dbAdapter = nullptr;
	cv::Mat decode(std::vector<int> values);
};

