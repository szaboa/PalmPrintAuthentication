#pragma once
#include <module_FeatureExtraction/IFeature.h>

class IMatcher
{
public:
	virtual int doMatching(IFeature* f) = 0;

	virtual ~IMatcher(){};
};

