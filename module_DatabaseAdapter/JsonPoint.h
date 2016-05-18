#pragma once
#include "json/json11.hpp"

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace json11;
using namespace cv;

class JsonPoint
{
public:
	int x, y;
	JsonPoint(Point p) : x(p.x), y(p.y){};
	Json to_json() const {
		return Json::array{x, y};
	};
	
};

