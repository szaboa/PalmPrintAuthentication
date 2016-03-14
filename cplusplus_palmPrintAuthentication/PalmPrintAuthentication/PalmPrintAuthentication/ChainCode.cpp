#include "ChainCode.h"
#include "Thinning.h"
#include <iostream>


std::string ChainCode::getChainCode(const Mat &img, std::vector<std::vector<Point>> lineComponents){
	

	std::cout << lineComponents.size() << std::endl;

	namedWindow("After thinning", WINDOW_AUTOSIZE);
	imshow("After thinning", img);
	imwrite("output.jpg", img);
	waitKey(0);

	return "asd";
}
