#pragma once
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <module_DatabaseAdapter\DbAdapter.h>
#include <module_Matcher\IMatcher.h>
#include <module_FeatureExtraction\IFeature.h>
#include <vector>
#include <iostream>


using namespace cv;

class PrincipalLineMatcher : public IMatcher
{
public:
	PrincipalLineMatcher();

	int doMatching(IFeature *f) override; 

	~PrincipalLineMatcher();
private:

	DbAdapter *dbAdapter = nullptr;

	Mat doDistanceTransformation(Mat img);

	
};

