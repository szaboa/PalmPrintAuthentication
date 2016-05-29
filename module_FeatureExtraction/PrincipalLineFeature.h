#pragma once
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <module_DatabaseAdapter/DbAdapter.h>
#include <module_FeatureExtraction/IFeature.h>

class PrincipalLineFeature : public IFeature
{
public:
	PrincipalLineFeature(cv::Mat img);

	void save(int userId) override;
    cv::Mat getFeature() override;

	~PrincipalLineFeature();

private:
	cv::Mat img;
	DbAdapter *dbAdapter = nullptr;
};

