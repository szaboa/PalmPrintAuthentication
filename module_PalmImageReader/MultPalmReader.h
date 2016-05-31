#pragma once

#include <string>
#include <vector>
#include <module_PalmImageReader/IPalmReader.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

/**
 * @brief The MultPalmReader class is an implementation of the IPalmReader interface, and designed to read multiple images
 */
class MultPalmReader : public IPalmReader
{
public:
    /**
     * @brief MultPalmReader
     */
    MultPalmReader();

    /**
     * @brief Initializes the reader
     * @param dirPath Path of the selected directory
     */
	void init(std::string dirPath) override;

    /**
     * @brief Reads the nex image from the given directory
     * @return The read image (rgb)
     */
	cv::Mat readPalmImage() override;

    /**
     * @brief Reads the user id based on the file name and returns it
     * @return The user id of the last read image
     */
	int readUserId() override;

    /**
     * @brief Checks if the directory has unread images
     * @return true if the directory has unread images, false otherwise
     */
	bool hasNextImage();

    /**
     * @brief Returns the number of images in the given directory
     * @return Number of images
     */
    int getNumberOfImages();

    ~MultPalmReader();

private:
    unsigned int iterator = 0;

    /**
     * @brief Path of the given directory
     */
    std::string dirPath;

    /**
     * @brief Storing image names which are in the given directory
     */
	std::vector<std::string> imageNames;
};

