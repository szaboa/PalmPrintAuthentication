#pragma once

#include <module_PalmImageReader\IPalmReader.h>
#include <string>
#include <vector>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class PalmReaderEnrollment : public IPalmReader
{
public:
	PalmReaderEnrollment();

	void init(std::string dirPath) override;

	cv::Mat readPalmImage() override;

	int readUserId() override;

	bool hasNextImage();

	~PalmReaderEnrollment();

private:
	int iterator = 0;
	std::string dirPath;
	std::vector<std::string> imageNames;
};

