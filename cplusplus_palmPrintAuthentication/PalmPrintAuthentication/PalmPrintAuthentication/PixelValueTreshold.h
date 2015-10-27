#pragma once
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
/*This singleton class contains thresholding methods*/
class PixelValueTreshold
{

private:
	/*private constructor */
	PixelValueTreshold(){};
	/*private copy constructor*/
	PixelValueTreshold(PixelValueTreshold const&){};
	/*private assignment operator*/
	PixelValueTreshold& operator = (PixelValueTreshold const&){};

public:
	

	/*If a pixel values is between @min and @max returns true, otherwise false*/
	static bool inRange(Scalar value, Scalar min, Scalar max);

};

