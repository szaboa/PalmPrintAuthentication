#pragma once
#include <module_FeatureExtraction/IFeature.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class IMatcher
{
public:
    virtual std::pair<double,int> doMatching(IFeature* f) = 0;
    virtual IFeature* getMatchedFeature()=0;
	virtual ~IMatcher(){};
};

