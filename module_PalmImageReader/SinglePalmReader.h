#pragma once
#include <module_PalmImageReader/IPalmReader.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

/**
 * @brief The SinglePalmReader class is an implementation of the IPalmReader interface, and designed to read a single image
 */
class SinglePalmReader : public IPalmReader
{
public:
    /**
     * @brief SinglePalmReader
     */
    SinglePalmReader();
	
    /**
     * @brief Initializes the reade
     * @param path Path of the selected image
     */
	void init(std::string path) override;

    /**
     * @brief Reads the image from the given path
     * @return The read image (rgb)
     */
	cv::Mat readPalmImage() override;

    /**
     * @brief Reads the user id based on the file name and returns it
     * @return The user id of the read image
     */
	int readUserId() override;

    ~SinglePalmReader();

private:
	std::string path;
};

