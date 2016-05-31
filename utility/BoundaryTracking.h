#pragma once

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>

using namespace cv;

/**
 * @brief The BoundaryTracking class is designed to perform boundary tracking algorithm on a given image
 */
class BoundaryTracking
{
public:
    /**
     * @brief Returns the boundary points in order
     * @param img Binary image
     * @return Vector of points, represents the boundary
     */
	static std::vector<Point> getBoundary(const Mat &img);

    /**
     * @brief Gives the next search direction
     * @param dir The current direction (1-8)
     * @return The next search direction
     */
	static int getNeighborhoodSearchIndex(int dir);

    /**
     * @brief Gives the next boundary point
     * @param img Binary image
     * @param dir Search direction
     * @param currentPoint Current point
     * @return Next boundary point
     */
	static Point getNextBoundaryPoint(const Mat &img, int &dir, Point currentPoint);
private:

	BoundaryTracking(){	};
	BoundaryTracking(BoundaryTracking const&){};
	BoundaryTracking& operator = (BoundaryTracking const&){};
};

