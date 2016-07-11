#pragma once
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include <iostream>
#include <string>
#include <module_FeatureExtraction/IFeatureExtractor.h>
#include <module_FeatureExtraction/IFeature.h>

using namespace cv;

/**
 * @brief The PrincipalLineExtractor class is an implementation of the IFeatureExtractor, and extracts the principal lines from the ROI
 */
class PrincipalLineExtractor : public IFeatureExtractor
{
public:

    /**
     * @brief PrincipalLineExtractor
     */
    PrincipalLineExtractor();

    /**
     * @brief Extracts the line features, based on the given ROI
     * @param ROI of the palm image
     * @return IFeature implementation that contains the extracted features
     */
	IFeature* doFeatureExtraction(Mat roi) override;

    ~PrincipalLineExtractor();

private:

    /**
     * @brief Stores the input ROI
     */
	Mat roi;

    /**
     * @brief Output image, contains the extracted lines
     */
	Mat finalLines;

    /**
     * @brief Defines the ROI's width and height
     */
	static const int roiWidth  = 128;
	static const int roiHeight = 128;

    /**
     * @brief Defines the range of the first-order derivative's change detection
     */
    int firstDerivChangeWidth = 4;

    /**
     * @brief Defines the second-order derivative's treshold value
     */
    int secondDerivTresholdValue = 10;

    /**
     * @brief Defines the minimum size of a component (Connected-component labeling)
     */
    int componentMinSize = 15;


    /**
     * @brief Normalizes the image  (using Adaptive Histogram Equalization and a Low-pass Filter)
     * @param img The original ROI
     * @return
     */
	Mat normalizeImage(Mat img);

    /**
     * @brief Checks if the given coordinate is inside the image
     * @param x x-coordinate
     * @param y y-coordinate
     * @return
     */
    bool isInsideTheBoundary(int x, int y);

    /**
     * @brief Check if the given pixel is foreground
     * @param img Image
     * @param p Examined pixel
     * @return true if the pixel belongs to foreground, false otherwise
     */
	bool isForeground(const Mat &img, Point p);

    /**
     * @brief Locates the final principal lines in four direction and merge them
     * @param Normalized ROI
     * @return Image contains the final principal lines
     */
    Mat locatePrincipalLines(Mat img);

    /**
     * @brief Locates principal lines in a given direction
     * @param img Normalized ROI
     * @param H1 Line detector matrix in a given direction
     * @param H2 Line detector matrix in a given direction
     * @param degree Defines the direction in degree (0,45,90,135)
     * @return The extracted lines in the given direction
     */
	Mat locatePrincipalLineInGivenDirection(const Mat &img, std::vector<std::vector<double>> H1, std::vector<std::vector<double>> H2, int degree);

    /**
     * @brief Removes components that are smaller than componentMinSize
     * @param Binary image, containing the extracted lines
     * @param Component size threshold
     * @return The filtered image, doesn't contain the removed components
     */
	Mat filterWithConnectedComponentLabeling(Mat img, int componentMinSize);
	
    /**
     * @brief Preprocessing of the ROI (resizes and converts to grayscale)
     * @param ROI
     * @return 128x128 grayscale image
     */
	Mat preprocessing(Mat img);


    /**
     * @brief Gets the neighbor points of the given point
     * @param currentPoint Given point
     * @return Neighbors of the given point
     */
	std::vector<Point> getNeighborPoints(Point currentPoint);

};

