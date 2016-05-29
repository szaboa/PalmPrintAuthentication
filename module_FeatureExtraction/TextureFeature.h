#pragma once
#include <module_FeatureExtraction/IFeature.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <module_DatabaseAdapter/DbAdapter.h>
#include <string>
#include <json/json11.hpp>

class TextureFeature : public IFeature
{
public:
	TextureFeature(cv::Mat realMat, cv::Mat imaginaryMat);
	void save(int userId) override;
    Mat getFeature() override;
	Mat getRealComponent();
	Mat getImaginaryComponent();
	~TextureFeature();
private:
	DbAdapter *dbAdapter = nullptr;

	unsigned char bitsToByte(std::string bits);

	Mat reMat, imMat;
};

