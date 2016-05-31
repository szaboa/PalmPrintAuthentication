#pragma once

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace cv;

/**
 * @brief The Thinning class is designed to do thinning operation on a given image
 */
class Thinning
{
public:
    /**
     * @brief Thinning a given image
     * @param img Binary image
     * @return Binary image, contains 1 pixel wide egdes
     */
	static Mat doThinning(const Mat img);
private:
	Thinning(){	};
	Thinning(Thinning const&){};
	Thinning& operator = (Thinning const&){};

	static void thinningIteration(Mat &img, int iteration);
};

