#include "PixelValueTreshold.h"

using namespace cv;

bool PixelValueTreshold::inRange(Scalar value, Scalar min, Scalar max){
	if (value.val[0] >= min.val[0] && value.val[1] >= min.val[1] && value.val[2] >= min.val[2] &&
		value.val[0] <= max.val[0] && value.val[1] <= max.val[1] && value.val[2] <= max.val[2]){
		return true;
	}
	else{
		return false;
	}
}