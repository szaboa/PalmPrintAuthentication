#pragma once

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <vector>

using namespace cv;

class BoundaryTracking
{
public:
	/* Returns the boundary points in order */
	static std::vector<Point> getBoundary(const Mat &img);

	static int getNeighborhoodSearchIndex(int dir);

	static Point getNextBoundaryPoint(const Mat &img, int &dir, Point currentPoint);
private:
	/*private constructor */
	BoundaryTracking(){	};
	/*private copy constructor*/
	BoundaryTracking(BoundaryTracking const&){};
	/*private assignment operator*/
	BoundaryTracking& operator = (BoundaryTracking const&){};
};

