#include "RegionSegmentation.h"
#include "Histogram.h"
#include "PixelValueTreshold.h"
#include <iostream>
#include <queue>
#include <math.h>

using namespace std;


Mat RegionSegmentation::applyRegionGrowingAlgorithm(Mat img){

	//Mat outputImage = img;
	Mat grayscaleMat(img.size(), CV_8U, Scalar(0,0,0));

	// get size of the original image
	Size imgSize = img.size();
	int width = imgSize.width;
	int height = imgSize.height;

	// allocating 'visited' array
	bool** visited = new bool*[width];
	for (int i = 0; i < width; ++i){
		visited[i] = new bool[height];
	}

	// init 'visited' array values
	for (int i = 0; i < width; ++i){
		for (int j = 0; j < height; ++j){
			visited[i][j] = false;
		}
	}

	// seed point
	Point seedPoint(width-1, height / 2);

	// points which needs to be evaluated
	queue<Point> processQueue;
	processQueue.push(seedPoint);
	int i = 0;

	visited[seedPoint.x][seedPoint.y] = true;

	Scalar minTresholdValue(0, Histogram::getMinValueChanel2() - 5, Histogram::getMinValueChanel3() -5 );
	Scalar maxTresholdValue(255, Histogram::getMaxValueChanel2() + 5, Histogram::getMaxValueChanel3() +5);


	while (!processQueue.empty()){
		Point currentPoint = processQueue.front();
		processQueue.pop();




		vector<Point> neighbourPoints = getNeighbourPoints(currentPoint);
		for (Point p : neighbourPoints){

			//if (p.x >= 0 && p.x < width && p.y >= 0 && p.y < height &&  visited[p.x][p.y] == false){
			if (p.x >= 0 && p.x < width && p.y >= 0 && p.y < height &&  visited[p.x][p.y] == false){
				Scalar pValue(
					img.at<Vec3b>(p.y, p.x)[0],
					img.at<Vec3b>(p.y, p.x)[1],
					img.at<Vec3b>(p.y, p.x)[2]);
				bool inRegion = PixelValueTreshold::inRange(pValue, minTresholdValue, maxTresholdValue);
				if (inRegion){
					grayscaleMat.at<uchar>(p.y, p.x) = 255;
					processQueue.push(p);
				}

				visited[p.x][p.y] = true;
			}
		}
	}


	// deallocating 'visited' array
	for (int i = 0; i < width; ++i){
		delete[] visited[i];
	}
	delete[] visited;

	return grayscaleMat;
}

vector<Point> RegionSegmentation::getNeighbourPoints(const Point &point){
	vector<Point> neighbourPoints;
	neighbourPoints.push_back(Point(point.x - 1, point.y - 1));
	neighbourPoints.push_back(Point(point.x, point.y - 1));
	neighbourPoints.push_back(Point(point.x + 1, point.y - 1));
	neighbourPoints.push_back(Point(point.x - 1, point.y));
	neighbourPoints.push_back(Point(point.x, point.y));
	neighbourPoints.push_back(Point(point.x + 1, point.y));
	neighbourPoints.push_back(Point(point.x - 1, point.y + 1));
	neighbourPoints.push_back(Point(point.x, point.y + 1));
	neighbourPoints.push_back(Point(point.x + 1, point.y + 1));

	return neighbourPoints;
}
