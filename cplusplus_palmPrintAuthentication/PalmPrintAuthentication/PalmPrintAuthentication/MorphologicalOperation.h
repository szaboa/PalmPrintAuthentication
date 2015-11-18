#pragma once
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
/*Singleton class for morphological operations*/
class MorphologicalOperation
{
private:
	/*private constructor */
	MorphologicalOperation(){	};
	/*private copy constructor*/
	MorphologicalOperation(MorphologicalOperation const&){};
	/*private assignment operator*/
	MorphologicalOperation& operator = (MorphologicalOperation const&){};

public:
	/*Applies dilate algorithm to @img */
	static Mat dilate(const Mat &img);

	/*Applies erode algorithm to @img */
	static Mat erode(const Mat &img);
};

