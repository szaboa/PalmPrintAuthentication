#pragma once
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <module_FeatureExtraction\IFeatureExtraction.h>
#include <module_FeatureExtraction\IFeature.h>


using namespace cv;

class TextureExtraction :public IFeatureExtraction	
{
public:
	TextureExtraction();

	std::pair<Mat,Mat> create2dGaborFitler(int kSize, double theta, double u, double sigma);

	IFeature* doFeatureExtraction(Mat roi) override;

	~TextureExtraction();
};

