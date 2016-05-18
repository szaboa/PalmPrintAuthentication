#pragma once
#include <module_PalmImageReader/IPalmReader.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class PalmReaderMatching : public IPalmReader
{
public:
	PalmReaderMatching();
	
	void init(std::string path) override;

	cv::Mat readPalmImage() override;

	int readUserId() override;

	~PalmReaderMatching();

private:
	std::string path;
};

