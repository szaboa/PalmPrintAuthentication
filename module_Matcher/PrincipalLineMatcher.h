#pragma once
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <module_DatabaseAdapter/DbAdapter.h>
#include <module_Matcher/IMatcher.h>
#include <module_FeatureExtraction/IFeature.h>
#include <vector>
#include <iostream>


using namespace cv;

class PrincipalLineMatcher : public IMatcher
{
public:
	PrincipalLineMatcher();

    std::pair<double,int> doMatching(IFeature *f) override;

    IFeature* getMatchedFeature() override;

	~PrincipalLineMatcher();
private:

	DbAdapter *dbAdapter = nullptr;

	Mat doDistanceTransformation(Mat img);

    IFeature* matchedFeature = nullptr;

	
};

