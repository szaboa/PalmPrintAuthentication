#pragma once
#include "json/json11.hpp"

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace json11;
using namespace cv;

/**
 * @brief The JsonPoint class converts a cv::Point to it's json representation
 */
class JsonPoint
{
public:
    /**
     * @brief x x-coordinate of the given point
     */
    int x;
    /**
     * @brief y y-coordinate of the given point
     */
    int y;

    /**
     * @brief Constructor, stores the x and y coordinates of the given point
     * @param p cv::Point
     */
	JsonPoint(Point p) : x(p.x), y(p.y){};

    /**
     * @brief Converts the stored point into json representation
     * @return Json array
     */
    Json to_json() const {
		return Json::array{x, y};
	};
	
};

