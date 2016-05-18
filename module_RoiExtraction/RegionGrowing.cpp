#include <module_RoiExtraction/RegionGrowing.h>
#include <utility/MorphologicalOperation.h>
#include <queue>
#include <iostream>

using namespace cv;
using namespace std;

RegionGrowing::RegionGrowing(){

};

Mat RegionGrowing::doPreprocessing(const Mat &inputImage){
	
	Mat centerOfImage = cropCenterOfImage(inputImage, CENTER_SIZE);


	// get an instance from Histogram
	histogram = new Histogram(centerOfImage);

	//split input image to YCbCr chanels
	Mat inputImageYCbCr;
	cvtColor(inputImage, inputImageYCbCr, COLOR_BGR2YCrCb);
	vector<Mat> ycbcr_planes;
	split(inputImageYCbCr, ycbcr_planes);


	// crop the right side of the image, reduce computation
	Rect rightSideRoi(0, 0, inputImage.cols / 2 + 200, inputImage.rows);
	Mat rightSide = inputImageYCbCr(rightSideRoi);

	int rightSideWidth = rightSide.size().width;
	int rightSideHeight = rightSide.size().height;
	seedPoint = Point(rightSideWidth - 1, rightSideHeight / 2);
	
	return rightSide;
}

Mat RegionGrowing::doRegionSegmentation(const Mat &preprocessedImage){

	//Mat outputImage = img;
	Mat grayscaleMat(preprocessedImage.size(), CV_8U, Scalar(0, 0, 0));

	// get size of the original image
	Size imgSize = preprocessedImage.size();
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

	// points which needs to be evaluated
	queue<Point> processQueue;
	processQueue.push(seedPoint);
	int i = 0;

	visited[seedPoint.x][seedPoint.y] = true;

	Scalar minTresholdValue(0, histogram->getMinValueChanel2() - 5, histogram->getMinValueChanel3() - 5);
	Scalar maxTresholdValue(255, histogram->getMaxValueChanel2() + 5, histogram->getMaxValueChanel3() + 5);


	while (!processQueue.empty()){
		Point currentPoint = processQueue.front();
		processQueue.pop();




		vector<Point> neighbourPoints = getNeighbourPoints(currentPoint);
		for (Point p : neighbourPoints){

			//if (p.x >= 0 && p.x < width && p.y >= 0 && p.y < height &&  visited[p.x][p.y] == false){
			if (p.x >= 0 && p.x < width && p.y >= 0 && p.y < height &&  visited[p.x][p.y] == false){
				Scalar pValue(
					preprocessedImage.at<Vec3b>(p.y, p.x)[0],
					preprocessedImage.at<Vec3b>(p.y, p.x)[1],
					preprocessedImage.at<Vec3b>(p.y, p.x)[2]);
				bool inRegion = inRange(pValue, minTresholdValue, maxTresholdValue);
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

	// Apply dilatation to fill the black holes
	grayscaleMat = MorphologicalOperation::dilate(grayscaleMat, 2);

	return grayscaleMat;
}

vector<Point> RegionGrowing::getNeighbourPoints(const Point &point){
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

bool RegionGrowing::inRange(Scalar value, Scalar min, Scalar max){
	if (value.val[0] >= min.val[0] && value.val[1] >= min.val[1] && value.val[2] >= min.val[2] &&
		value.val[0] <= max.val[0] && value.val[1] <= max.val[1] && value.val[2] <= max.val[2]){
		return true;
	}
	else{
		return false;
	}
}


Mat RegionGrowing::cropCenterOfImage(const Mat &image, int centerSize){
	Size inputImageSize = image.size();

	Rect roi = Rect((inputImageSize.width) / 2 + 200 - centerSize / 2, (inputImageSize.height) / 2 - centerSize / 2, centerSize, centerSize);
	Mat croppedImage = image(roi);

	Mat croppedImageYCbCr;
	cvtColor(croppedImage, croppedImageYCbCr, CV_BGR2YCrCb);

	return croppedImageYCbCr;
}

RegionGrowing::~RegionGrowing(){
	delete histogram;
}
