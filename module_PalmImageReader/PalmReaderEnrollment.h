#pragma once


#include <string>
#include <vector>
#include <module_PalmImageReader/IPalmReader.h>

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

    int getNumberOfImages();

	~PalmReaderEnrollment();

private:
    unsigned int iterator = 0;
	std::string dirPath;
	std::vector<std::string> imageNames;
};

