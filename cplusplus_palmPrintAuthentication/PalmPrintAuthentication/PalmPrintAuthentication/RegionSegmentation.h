#pragma once
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <vector>

using namespace cv;

/*This singleton class contains region based image segmentation*/
class RegionSegmentation
{
private:
	/*private constructor */
	RegionSegmentation(){	};
	/*private copy constructor*/
	RegionSegmentation(RegionSegmentation const&){};
	/*private assignment operator*/
	RegionSegmentation& operator = (RegionSegmentation const&){};

public:

	static void applyRegionGrowingAlgorithmEucDistance(Mat img);
	/*Applies region growing algorithm on @img, with (0,0) seed point */
	static Mat applyRegionGrowingAlgorithm(Mat img);

	/*Returns a vector of Point2i objects, that are neighbours with @point (8 direction)*/
	static std::vector<Point> getNeighbourPoints(const Point &point);
};

