#pragma once

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"


using namespace cv;

class Thinning
{
public:
	static Mat doThinning(const Mat img);
private:
	/*private constructor */
	Thinning(){	};
	/*private copy constructor*/
	Thinning(Thinning const&){};
	/*private assignment operator*/
	Thinning& operator = (Thinning const&){};

	static void thinningIteration(Mat &img, int iteration);
};

