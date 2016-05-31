#include <module_RoiExtraction/SquareRoiExtractor.h>
#include <module_Preprocessing/IPreprocessor.h>
#include <utility/BoundaryTracking.h>
#include <vector>
#include <algorithm>
#include <math.h>
#include <utility/PPAException.h>
#include <fstream>
#include "easylogging++.h"

using namespace cv;
using namespace std;

/**
 * @brief Struct for holding a boundary point and it's distance from center point
 */
struct Point_with_distance{
    /**
     * @brief Boundary point
     */
	Point point;

    /**
     * @brief Distance from boundary point to the center point
     */
	double distance;
};

SquareRoiExtractor::SquareRoiExtractor(Mat image, IPreprocessor* preprocessor)
{
    this->inputImage = image;
    this->preprocessor = preprocessor;
}

Mat SquareRoiExtractor::doExtraction(){
    resize(inputImage, inputImage, Size(640, 480));
    inputImageCopy = inputImage.clone();

    Mat extractedRoi;

    // Segment the hand using Skin-Color model algorithm
    Mat segmentedImage = preprocessor->doPreprocessing(inputImage);

    try{
        Keypoints struct_keypoints = findKeypoints(segmentedImage);
        extractedRoi = calcSquareRoi(struct_keypoints);

	}
	catch (PPAException &e){
        LOG(INFO) << e.what();
        throw e;
	}

    return extractedRoi;
}


Mat SquareRoiExtractor::calcSquareRoi(const Keypoints &keypoints){

    // Draw a line between the first and third keypoint (for debugging)
	line(inputImage, keypoints.keypoint1, keypoints.keypoint3, Scalar(0, 0, 255), 1);

	// Calculating the perpendicular slope to the previous line
	double slope = 0.0;
	double perpendicular_slope = 0.0;

	if (keypoints.keypoint3.x - keypoints.keypoint1.x == 0){
		perpendicular_slope = 0;
	}
	else{
        slope = (double)(keypoints.keypoint3.y - keypoints.keypoint1.y) / (double)(keypoints.keypoint3.x - keypoints.keypoint1.x);
		perpendicular_slope = -1 / slope;
	}


	// Calculating the ROI's top left and bottom left points, 
	// by finding the lines which goes through these points and are perpendicular to the line between keypoint1 and keypoint3

	Point keypoint1_1, keypoint3_1;
	double dist = sqrt(pow(keypoints.keypoint1.x - keypoints.keypoint3.x, 2) + pow(keypoints.keypoint1.y - keypoints.keypoint3.y, 2));
	keypoint1_1.x = keypoints.keypoint1.x + dist;
	keypoint3_1.x = keypoints.keypoint3.x + dist;


	keypoint1_1.y = perpendicular_slope*(keypoint1_1.x - keypoints.keypoint1.x) + keypoints.keypoint1.y;
    keypoint3_1.y = perpendicular_slope*(keypoint3_1.x - keypoints.keypoint3.x) + keypoints.keypoint3.y;

    // Drawing the square ROI (for debugging)
	line(inputImage, keypoints.keypoint1, keypoint1_1, Scalar(0, 0, 255), 1);
	line(inputImage, keypoints.keypoint3, keypoint3_1, Scalar(0, 0, 255), 1);
	line(inputImage, keypoint1_1, keypoint3_1, Scalar(0, 0, 255), 1);

	// Calculating the rotation angle before cropping the square roi
	double rotAlpha = 0.0;

	if (keypoints.keypoint3.y - keypoints.keypoint1.y != 0){
		rotAlpha = tan((double)(keypoints.keypoint3.x - keypoints.keypoint1.x) / (double)(keypoints.keypoint3.y - keypoints.keypoint1.y));
	}

	
	// Rotating the input image
	Point2f centerPoint(inputImage.cols / 2.0F, inputImage.rows / 2.0F);

	Mat rotMat = getRotationMatrix2D(centerPoint, -rotAlpha*(180 / 3.1415), 1.0);

	warpAffine(inputImage, inputImage, rotMat, inputImage.size());
	warpAffine(inputImageCopy, inputImageCopy, rotMat, inputImage.size());

	// Getting the new keypoints by rotating the old ones with rotAlpha angle

	double keypoint3x = keypoint3_1.x - inputImage.cols / 2;
	double keypoint3y = keypoint3_1.y - inputImage.rows / 2;

	double keypoint1x = keypoints.keypoint1.x - inputImage.cols / 2;
	double keypoint1y = keypoints.keypoint1.y - inputImage.rows / 2;

	Point newUpperLeftCorner;
	Point newBottomRightCorner;

	newBottomRightCorner.x = keypoint3x * cos(-rotAlpha) + keypoint3y * sin(-rotAlpha);
	newBottomRightCorner.y = (-keypoint3x) * sin(-rotAlpha) + keypoint3y * cos(-rotAlpha);

	newBottomRightCorner.x = newBottomRightCorner.x + inputImage.cols / 2;
	newBottomRightCorner.y = newBottomRightCorner.y + inputImage.rows / 2;


	newUpperLeftCorner.x = keypoint1x * cos(-rotAlpha) + keypoint1y * sin(-rotAlpha);
	newUpperLeftCorner.y = (-keypoint1x) * sin(-rotAlpha) + keypoint1y * cos(-rotAlpha);

	newUpperLeftCorner.x = newUpperLeftCorner.x + inputImage.cols / 2;
	newUpperLeftCorner.y = newUpperLeftCorner.y + inputImage.rows / 2;

    // (for debugging)
	circle(inputImage, newBottomRightCorner, 3, Scalar(255, 0, 0), 1);
	circle(inputImage, newUpperLeftCorner, 3, Scalar(255, 0, 0), 1);
	
    //Extract the square ROI
    return inputImageCopy(Rect(newUpperLeftCorner, newBottomRightCorner));
}

Keypoints SquareRoiExtractor::findKeypoints(const Mat &segmentedImage){

	Mat leftSide = segmentedImage(Rect(0,0,segmentedImage.cols-150,segmentedImage.rows));
    vector<Point> boundaryVector;
    try{
        // Get boundary points, by applying boundary tracking alogirthm
         boundaryVector = BoundaryTracking::getBoundary(leftSide);

    }catch(PPAException& e){
        throw;
    }

	// Find start and end points (vertically) at left side of the image
    Point startPoint(-1,-1), endPoint(-1,-1);

	bool foundStart = false;
	for (int i = 0; i < leftSide.rows; ++i){
		if (foundStart){
			if (leftSide.at<uchar>(i, leftSide.cols - 1) == 0){
				startPoint.x = leftSide.cols - 1;
				startPoint.y = i;
				break;
			}
		}
		else{
			if (leftSide.at<uchar>(i, leftSide.cols - 1) == 255){
				foundStart = true;
				endPoint.x = leftSide.cols - 1;
				endPoint.y = i;
			}
		}
	}

    //Check if startPoint and endPoint was successfuly found
    if(startPoint.x == -1 && startPoint.y == -1 || endPoint.x == -1 && endPoint.y == -1){
        throw PPAException("Keypoints not found.");
    }

	// Find the center point (vertically) at right side of the image
	Point centerPoint(leftSide.cols, (startPoint.y + endPoint.y) / 2);
	circle(inputImage, centerPoint, 2, CV_RGB(0, 255, 0), -1);

	// Calculate dinstances between the center points and every boundary point 
	vector<Point_with_distance> distancesFromCenterPoint;

	for (int i = 0; i < boundaryVector.size(); ++i){
		Point_with_distance p;
		p.point.x = boundaryVector.at(i).x;
		p.point.y = boundaryVector.at(i).y;

		p.distance = sqrt(pow(centerPoint.x - p.point.x, 2) + pow(centerPoint.y - p.point.y, 2));
		distancesFromCenterPoint.push_back(p);
	}

	// Finding the local minimums on the distance function, these will be keypoints
	// Applying a Mean filter on distance function, to get rid of false local minimums

	vector<Point_with_distance> filtered_distances = distancesFromCenterPoint;

	double sum = 0.0;
    double kernel_size = 5;
	
    for (int index = 2; index < distancesFromCenterPoint.size() - 2; index++){
		sum = distancesFromCenterPoint.at(index - 2).distance + distancesFromCenterPoint.at(index - 1).distance + distancesFromCenterPoint.at(index).distance + distancesFromCenterPoint.at(index + 1).distance + distancesFromCenterPoint.at(index + 2).distance;
		filtered_distances.at(index).distance = sum / kernel_size;
	}
	
	// Searcing for local minimums, with a predefined stepsize,
	// Check if the stepsize'th previous point is bigger than the current points and the stepsize'th next point is also bigger than the current point
	// If the previous condition is true, then the current point will be stored as a local minimum

	int stepsize = 50;
	vector<Point_with_distance> minimums;

	if (filtered_distances.size() < 500) throw PPAException("Boundary not found.");

	for (int i = stepsize; i < filtered_distances.size() - stepsize; ++i){
		if (filtered_distances.at(i - stepsize).distance > filtered_distances.at(i).distance && filtered_distances.at(i + stepsize).distance > filtered_distances.at(i).distance)
		{
			Point_with_distance temp;
			temp.point = filtered_distances.at(i).point;
			temp.distance = filtered_distances.at(i).distance;
			
			minimums.push_back(temp);
		}
	}


	// Creating three clusters, these will contain local minimum points around the keypoints (valleys between fingers)
	// Iterating through the local minimums, if the distance is higher than 10 pixels between the next and current point, then the next point will be classified into a new cluster,
	// otherwise, it will be classified into the current cluster

	vector<vector<Point_with_distance>> min_clusters;
	vector<Point_with_distance> temp_row;
	for (int i = 0; i < minimums.size() - 1; ++i){
		Point_with_distance current, next;
		current = minimums.at(i);
		next = minimums.at(i + 1);
		float dist = sqrt(pow(current.point.x - next.point.x, 2) + pow(current.point.y - next.point.y, 2));
		if (dist > 10){
			min_clusters.push_back(temp_row);
			temp_row.clear();
		}
		else{
			temp_row.push_back(current);
		}
	}

	if (temp_row.size() != 0) min_clusters.push_back(temp_row);

	// Returning this struct
	Keypoints keypoints;

	// If the number of the clusters isn't four, then something went wrong
	if (min_clusters.size() != 4) {
		keypoints.success = false;
		throw PPAException("Keypoints not found.");
	}
	else{

		// Search minimums in the clusters, based on the distance from the center point. These will be the keypoints.

		Point_with_distance keypoint1, keypoint2, keypoint3;
		keypoint1 = *min_element(min_clusters[1].begin(), min_clusters[1].end(), [](const Point_with_distance& x, const Point_with_distance& y) {return x.distance < y.distance; });
		circle(inputImage, keypoint1.point, 2, CV_RGB(255, 0, 0), -1);

		keypoint2 = *min_element(min_clusters[2].begin(), min_clusters[2].end(), [](const Point_with_distance& x, const Point_with_distance& y) {return x.distance < y.distance; });
		circle(inputImage, keypoint2.point, 2, CV_RGB(0, 255, 0), -1);

		keypoint3 = *min_element(min_clusters[3].begin(), min_clusters[3].end(), [](const Point_with_distance& x, const Point_with_distance& y) {return x.distance < y.distance; });
		circle(inputImage, keypoint3.point, 2, CV_RGB(0, 0, 255), -1);


		keypoints.centerPoint = centerPoint;
		keypoints.keypoint1 = keypoint1.point;
		keypoints.keypoint2 = keypoint2.point;
		keypoints.keypoint3 = keypoint3.point;
		keypoints.success = true;
	}

	return keypoints;
}


SquareRoiExtractor::~SquareRoiExtractor()
{
	
}
