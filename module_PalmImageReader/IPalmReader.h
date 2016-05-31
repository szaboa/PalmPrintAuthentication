#pragma once

#include <string>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

/**
 * @brief The IPalmReader class is an interface for reading images
 */
class IPalmReader
{
public:
    /**
     * @brief Initializes the reader
     * @param path Path to image(s) location
     */
	virtual void init(std::string path) = 0;

    /**
     * @brief Reads the image from the initialized path
     * @return The read image (rgb)
     */
	virtual cv::Mat readPalmImage() = 0;

    /**
     * @brief Reads the user id based on the file name and returns it
     * @return The user id of the last read image
     */
	virtual int readUserId() = 0;

    /**
     * @brief ~IPalmReader
     */
	virtual ~IPalmReader(){};
};

