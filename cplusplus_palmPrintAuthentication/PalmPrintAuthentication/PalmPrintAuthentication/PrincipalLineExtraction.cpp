#include "PrincipalLineExtraction.h"
#include <fstream>
#include <stack>

using namespace std;
using namespace cv;


PrincipalLineExtraction::PrincipalLineExtraction(Mat roi) : roi(roi)
{
	// Convert to grayscale
	cvtColor(roi, this->roi, CV_BGR2GRAY);

	// Resize it to 128x128
	resize(this->roi, this->roi, Size(roiWidth, roiHeight));

	// Show original ROI
	namedWindow("Before normalization", CV_WINDOW_AUTOSIZE);
	imshow("Before normalization", this->roi);

	// Image normalization (using Histogram Equalization and a Low-pass Filter)
	normalizeImage(this->roi);

	// Show ROI after normalization
	namedWindow("After normalization", CV_WINDOW_AUTOSIZE);
	imshow("After normalization", this->roi);

	// Locate principal lines
	locatePrincipalLines();

	waitKey(0);
}

void PrincipalLineExtraction::normalizeImage(Mat &img){

	/* Applying CLAHE */
	Ptr<CLAHE> clahe = createCLAHE();
	clahe->setClipLimit(4);	
	clahe->apply(img, img);

	/* Applying a low-pass filter */
	blur(img, img, Size(3, 3), Point(-1, -1));
}

Mat PrincipalLineExtraction::locatePrincipalLineInGivenDirection(const Mat &img, std::vector<std::vector<double>> H1, std::vector<std::vector<double>> H2, int degree){


	int H1_rows = H1.size();
	int H1_cols = H1[0].size();


	// First derivative of the input image
	double I_der[roiHeight][roiWidth];

	// Second derivate of the input image
	double I_der2[roiHeight][roiWidth];


	// Initialize first- and second-order derivatives
	for (int i = 0; i < roiHeight; ++i){
		for (int j = 0; j < roiWidth; ++j){
			I_der[i][j]  = 0.0;
			I_der2[i][j] = 0.0;
		}
	}

	// Calculate first- and second-order derivatives
	for (int i = H1_rows / 2; i < roiHeight - (H1_rows / 2); ++i){
		for (int j = H1_cols / 2; j < roiWidth - (H1_cols / 2); ++j){

			for (int _i = -(H1_rows / 2); _i <= (H1_rows / 2); ++_i){
				for (int _j = -(H1_cols / 2); _j <= +(H1_cols / 2); ++_j){
					I_der[i][j]  += static_cast<int>(img.at<uchar>(i + _i, j + _j)) * H1[_i + (H1_rows / 2)][_j + (H1_cols / 2)];
					I_der2[i][j] += static_cast<int>(img.at<uchar>(i + _i, j + _j)) * H2[_i + (H1_rows / 2)][_j + (H1_cols / 2)];
				}
			}
		}
	}

	// Create binary image, this will contain the extracted principal lines in a given direction
	Mat binaryImage(this->roi.size(), CV_8UC1, Scalar(0,0,0));
	
	// Locating changes in first-order derivatives in 0 direction
	if (degree == 0){

		// Traversing the matrix column by column
		for (int j = 3; j < 124; ++j){
			for (int i = 1; i < 126; ++i){

				bool derivChangeFound = false;

				// Checking if the derivative's sign has changed by comparing #dw neighbors 
				for (int dw = 1; dw < firstDerivChangeWidth; ++dw){
					if (isInsideTheBoundary(i - dw, j)){
						if ((I_der[i][j] * I_der[i-dw][j]) < 0){
							derivChangeFound = true;
						}
					}
				}
				/* If first-order derivative's sign has changed and the second-order derivative's value is greater 
				   than the treshold, then setting the current pixel to 255, otherwise to 0 */
				if ((derivChangeFound || I_der[i][j] == 0) && I_der2[i][j] > secondDerivTresholdValue){
					binaryImage.at<uchar>(i, j) = 255;
				}
				else{
					binaryImage.at<uchar>(i, j) = 0;
				}
			}
		}
	}

	// Locating changes in first-order derivatives in 90 direction
	if (degree == 90){

		// Traversing the matrix row by row
		for (int i = 1; i < 126; ++i){
			for (int j = 3; j < 124; ++j){

				bool derivChangeFound = false;
				// Checking if the derivative's sign has changed by comparing #dw neighbors 
				for (int dw = 1; dw < firstDerivChangeWidth; ++dw){
					if (isInsideTheBoundary(i, j - dw)){
						if ((I_der[i][j] * I_der[i][j - dw]) < 0){
							derivChangeFound = true;
						}
					}
				}
				/* If first-order derivative's sign has changed and the second-order derivative's value is greater
				than the treshold, then setting the current pixel to 255, otherwise to 0 */
				if ((derivChangeFound || I_der[i][j] == 0) && I_der2[i][j] > secondDerivTresholdValue){
					binaryImage.at<uchar>(i, j) = 255;
				}
				else{
					binaryImage.at<uchar>(i, j) = 0;
				}
			}
		}
	}


	// Locating changes in first-order derivatives in 45 direction
	if (degree == 45){

		// Traversing the matrix diagonal (bottom-left to top-right)
		for (int i = 1; i < 2 * 128 - 2; ++i){
			int k = i < 128 ? 0 : i - 128 + 1;
			for (int j = k+1; j <= i - k; ++j){

				bool derivChangeFound = false;
				// Checking if the derivative's sign has changed by comparing #dw neighbors 
				for (int dw = 1; dw < firstDerivChangeWidth; ++dw){
					if (isInsideTheBoundary(j - dw, i - j)){
						if ((I_der[j][i - j] * I_der[j - dw][i - j]) < 0){
							derivChangeFound = true;
						}
					}
				}
				/* If first-order derivative's sign has changed and the second-order derivative's value is greater
				than the treshold, then setting the current pixel to 255, otherwise to 0 */
				if ((derivChangeFound || I_der[j][i - j] == 0) && I_der2[j][i - j] > secondDerivTresholdValue){
					binaryImage.at<uchar>(j, i - j) = 255;
				}
				else{
					binaryImage.at<uchar>(j, i - j) = 0;
				}	
			}
		}
	}

	// Locating changes in first-order derivatives in 45 direction
	if (degree == 135){

		// Traversing the matrix diagonal (bottom-right to top-left)
		for (int i = 1; i < 2 * 128 - 2; ++i){
			int k = i < 128 ? 0 : i - 128 + 1;
			for (int j = k + 1; j <= i - k; ++j){

				int index = (128 - 1) - (i - j);
				bool derivChangeFound = false;
				// Checking if the derivative's sign has changed by comparing #dw neighbors 
				for (int dw = 1; dw < firstDerivChangeWidth; ++dw){
					if (isInsideTheBoundary(j - dw, index)){
						if ((I_der[j][index] * I_der[j - dw][index]) < 0){
							derivChangeFound = true;
						}
					}
				}

				/* If first-order derivative's sign has changed and the second-order derivative's value is greater
				than the treshold, then setting the current pixel to 255, otherwise to 0 */
				if ((derivChangeFound || I_der[j][index] == 0) && I_der2[j][index] > secondDerivTresholdValue){
					binaryImage.at<uchar>(j, index) = 255;
				}
				else{
					binaryImage.at<uchar>(j, index) = 0;
				}
			}
		}
	}
	return binaryImage;
}

void PrincipalLineExtraction::locatePrincipalLines(){


	// (1-D Gaussian function) x (first-order derivative of 1-D Gaussian function) 

	// Directional line detector in 0 direction
	vector<vector<double>> H1_0 = 
	{
		{ 0.0009, 0.0027, 0.0058, 0.0092, 0.0107, 0.0092, 0.0058, 0.0027, 0.0009 },
		{ 0.0065, 0.0191, 0.0412, 0.0655, 0.0764, 0.0655, 0.0412, 0.0191, 0.0065 },
		{ 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000 },
		{ -0.0009, -0.0027, -0.0058, -0.0092, -0.0107, -0.0092, -0.0058, -0.0027, -0.0009 },
		{ -0.0065, -0.0191, -0.0412, -0.0655, -0.0764, -0.0655, -0.0412, -0.0191, -0.0065 }
	};

	// Directional line detector in 45 direction 
	vector<vector<double>> H1_45 = 
	{ 
		{ -0.000900, -0.006500, 0.000000, 0.006500, 0.000900, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000 },
		{ 0.000000, -0.002700, -0.019100, 0.000000, 0.019100, 0.002700, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000 },
		{ 0.000000, 0.000000, -0.005800, -0.041200, 0.000000, 0.041200, 0.005800, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000 },
		{ 0.000000, 0.000000, 0.000000, -0.009200, -0.065500, 0.000000, 0.065500, 0.009200, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000 },
		{ 0.000000, 0.000000, 0.000000, 0.000000, -0.010700, -0.076400, 0.000000, 0.076400, 0.010700, 0.000000, 0.000000, 0.000000, 0.000000 },
		{ 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, -0.009200, -0.065500, 0.000000, 0.065500, 0.009200, 0.000000, 0.000000, 0.000000 },
		{ 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, -0.005800, -0.041200, 0.000000, 0.041200, 0.005800, 0.000000, 0.000000 },
		{ 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, -0.002700, -0.019100, 0.000000, 0.019100, 0.002700, 0.000000 },
		{ 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, -0.000900, -0.006500, 0.000000, 0.006500, 0.000900 }
	};

	// Directional line detector in 90 direction
	vector<vector<double>> H1_90 = 
	{ 
		{ -0.000900, -0.006500, 0.0, 0.006500, 0.000900 },
		{ -0.002700, -0.019100, 0.0, 0.019100, 0.002700 },
		{ -0.005800, -0.041200, 0.0, 0.041200, 0.005800 },
		{ -0.009200, -0.065500, 0.0, 0.065500, 0.009200 },
		{ -0.010700, -0.076400, 0.0, 0.076400, 0.010700 },
		{ -0.009200, -0.065500, 0.0, 0.065500, 0.009200 },
		{ -0.005800, -0.041200, 0.0, 0.041200, 0.005800 },
		{ -0.002700, -0.019100, 0.0, 0.019100, 0.002700 },
		{ -0.000900, -0.006500, 0.0, 0.006500, 0.000900 }
	};

	// Directional line detector in 135 direction
	vector<vector<double>> H1_135 = 
	{ 
		{ 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, -0.000900, -0.006500, 0.000000, 0.006500, 0.000900 },
		{ 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, -0.002700, -0.019100, 0.000000, 0.019100, 0.002700, 0.000000 },
		{ 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, -0.005800, -0.041200, 0.000000, 0.041200, 0.005800, 0.000000, 0.000000 },
		{ 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, -0.009200, -0.065500, 0.000000, 0.065500, 0.009200, 0.000000, 0.000000, 0.000000 },
		{ 0.000000, 0.000000, 0.000000, 0.000000, -0.010700, -0.076400, 0.000000, 0.076400, 0.010700, 0.000000, 0.000000, 0.000000, 0.000000 },
		{ 0.000000, 0.000000, 0.000000, -0.009200, -0.065500, 0.000000, 0.065500, 0.009200, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000 },
		{ 0.000000, 0.000000, -0.005800, -0.041200, 0.000000, 0.041200, 0.005800, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000 },
		{ 0.000000, -0.002700, -0.019100, 0.000000, 0.019100, 0.002700, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000 },
		{ -0.000900, -0.006500, 0.000000, 0.006500, 0.000900, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000 }
	};

	// 1-D Gaussian function x second-order derivative of 1-D Gaussian function

	// Directional line detector in 0 direction
	vector<vector<double>> H2_0 = { 
		{ 0.0156, 0.0211, 0.0309, 0.0416, 0.0464, 0.0416, 0.0309, 0.0211, 0.0156 },
		{ 0.0257, 0.0510, 0.0954, 0.1441, 0.1660, 0.1441, 0.0954, 0.0510, 0.0257 },
		{ -0.0298, -0.1125, -0.2582, -0.4178, -0.4896, -0.4178, -0.2582, -0.1125, -0.0298 },
		{ 0.0257, 0.0510, 0.0954, 0.1441, 0.1660, 0.1441, 0.0954, 0.0510, 0.0257 },
		{ 0.0156, 0.0211, 0.0309, 0.0416, 0.0464, 0.0416, 0.0309, 0.0211, 0.0156 }
	};

	// Directional line detector in 45 direction
	vector<vector<double>> H2_45 = 
	{ 
		{ 0.015600, 0.025700, -0.029800, 0.025700, 0.015600, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000 },
		{ 0.000000, 0.021100, 0.051000, -0.112500, 0.051000, 0.021100, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000 },
		{ 0.000000, 0.000000, 0.030900, 0.095400, -0.258200, 0.095400, 0.030900, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000 },
		{ 0.000000, 0.000000, 0.000000, 0.041600, 0.144100, -0.417800, 0.144100, 0.041600, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000 },
		{ 0.000000, 0.000000, 0.000000, 0.000000, 0.046400, 0.166000, -0.489600, 0.166000, 0.046400, 0.000000, 0.000000, 0.000000, 0.000000 },
		{ 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.041600, 0.144100, -0.417800, 0.144100, 0.041600, 0.000000, 0.000000, 0.000000 },
		{ 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.030900, 0.095400, -0.258200, 0.095400, 0.030900, 0.000000, 0.000000 },
		{ 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.021100, 0.051000, -0.112500, 0.051000, 0.021100, 0.000000 },
		{ 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.015600, 0.025700, -0.029800, 0.025700, 0.015600 }
	};

	// Directional line detector in 90 direction
	vector<vector<double>> H2_90 = 
	{ 
		{ 0.015600, 0.025700, -0.029800, 0.025700, 0.015600 },
		{ 0.021100, 0.051000, -0.112500, 0.051000, 0.021100 },
		{ 0.030900, 0.095400, -0.258200, 0.095400, 0.030900 },
		{ 0.041600, 0.144100, -0.417800, 0.144100, 0.041600 },
		{ 0.046400, 0.166000, -0.489600, 0.166000, 0.046400 },
		{ 0.041600, 0.144100, -0.417800, 0.144100, 0.041600 },
		{ 0.030900, 0.095400, -0.258200, 0.095400, 0.030900 },
		{ 0.021100, 0.051000, -0.112500, 0.051000, 0.021100 },
		{ 0.015600, 0.025700, -0.029800, 0.025700, 0.015600 }
	};

	// Directional line detector in 135 direction
	vector<vector<double>> H2_135 = 
	{ 
		{ 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.015600, 0.025700, -0.029800, 0.025700, 0.015600 },
		{ 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.021100, 0.051000, -0.112500, 0.051000, 0.021100, 0.000000 },
		{ 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.030900, 0.095400, -0.258200, 0.095400, 0.030900, 0.000000, 0.000000 },
		{ 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.041600, 0.144100, -0.417800, 0.144100, 0.041600, 0.000000, 0.000000, 0.000000 },
		{ 0.000000, 0.000000, 0.000000, 0.000000, 0.046400, 0.166000, -0.489600, 0.166000, 0.046400, 0.000000, 0.000000, 0.000000, 0.000000 },
		{ 0.000000, 0.000000, 0.000000, 0.041600, 0.144100, -0.417800, 0.144100, 0.041600, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000 },
		{ 0.000000, 0.000000, 0.030900, 0.095400, -0.258200, 0.095400, 0.030900, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000 },
		{ 0.000000, 0.021100, 0.051000, -0.112500, 0.051000, 0.021100, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000 },
		{ 0.015600, 0.025700, -0.029800, 0.025700, 0.015600, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000 }
	};

	// Locating principal lines in four directions
	Mat linesInDir0   = locatePrincipalLineInGivenDirection(this->roi, H1_0, H2_0, 0);
	Mat linesInDir90  = locatePrincipalLineInGivenDirection(this->roi, H1_90, H2_90, 90);
	Mat linesInDir45  = locatePrincipalLineInGivenDirection(this->roi, H1_45, H2_45, 45);
	Mat linesInDir135 = locatePrincipalLineInGivenDirection(this->roi, H1_135, H2_135, 135);

	// Merging the lines using OR operation
	Mat finalLines = linesInDir45 | linesInDir90 | linesInDir0 | linesInDir135;

	// Apply connected-component labeling to remove the irrelevant components
	connectedComponentLabeling(finalLines);

	// Show the final result
	namedWindow("Lines", WINDOW_AUTOSIZE);
	imshow("Lines", finalLines);
	waitKey(0);
}

void PrincipalLineExtraction::connectedComponentLabeling(Mat &img){

	// Defines the current label
	int currentLabel = 0;

	// A label matrix to track which pixel is already labeled
	int labels[128][128] = { { 0 } };

	// Using a stack to track the components (one component at time algorithm)
	stack<Point> pixelStack;

	// This vector will contain the separated components
	vector<vector<Point>> components;

	// travel the image row by row
	for (int x = 0; x < img.rows; ++x){
		for (int y = 0; y < img.cols; ++y){
			if (isForeground(img, Point(x,y)) && labels[x][y] == 0){
				currentLabel++;

				// Creating the current component
				vector<Point> currentComponent;
				
				// Labeling the current pixel
				labels[x][y] = currentLabel;

				// Pushing the current pixel to the stack
				pixelStack.push(Point(x, y));

				// Pushing the current pixel to the current component
				currentComponent.push_back(Point(x, y));

				while (!pixelStack.empty()){
					Point currentPixel = pixelStack.top();
					pixelStack.pop();

					// Getting the neighbor pixels of the current pixel
					vector<Point> neighborPoints = getNeighborPoints(currentPixel);

					// Iterating over the neigbor pixels
					for (Point neighborPoint : neighborPoints){

						// Checking if the neighbor pixel is foreground and it is not labeled yet
						if (isForeground(img, neighborPoint) && labels[neighborPoint.x][neighborPoint.y] == 0){

							// Labeling the neighbor pixel
							labels[neighborPoint.x][neighborPoint.y] = 1;

							// Pushing the neighbor pixel to the current component
							currentComponent.push_back(neighborPoint);

							// Pushing the neighbor pixel to the stack
							pixelStack.push(neighborPoint);
						}
					}

					if (pixelStack.empty()) components.push_back(currentComponent);
				}

				
			}
		}
	}

	// Removing components that are smaller than @componentMinSize	
	for (int i = 0; i < components.size(); ++i){
		if (components[i].size() < componentMinSize){
			for (Point p : components[i]){
				img.at<uchar>(p.x, p.y) = 0;
			}
		}
	}
	
	
}

void PrincipalLineExtraction::edgeLinking(Mat &img){
	int currentLabel = 0;
	int labels[128][128] = { { 0 } };
	stack<Point> pixelStack;
	vector<vector<Point>> components;

	vector<Point> nonZeroPoints;
	findNonZero(img, nonZeroPoints);

	Mat forOtsu(1, nonZeroPoints.size(), CV_8UC1, Scalar(0, 0, 0));
	for (int i = 0; i<nonZeroPoints.size(); ++i){
		forOtsu.at<uchar>(0, i) = img.at<uchar>(nonZeroPoints[i]);
	}
	double upperLimit = threshold(forOtsu, forOtsu, 0, 255, THRESH_OTSU);

	cout << "Otsu: " << upperLimit;
	// marking strong edges
	threshold(img, img, upperLimit, 255, THRESH_BINARY);

	// travel the image row by row
	for (int x = 0; x < img.rows; ++x){
		for (int y = 0; y < img.cols; ++y){
			if (isForeground(img, Point(x, y)) && labels[x][y] == 0){

				labels[x][y] = 1;
				pixelStack.push(Point(x, y));


				while (!pixelStack.empty()){
					Point currentPixel = pixelStack.top();
					pixelStack.pop();
					vector<Point> neighborPoints = getNeighborPoints(currentPixel);

					for (Point neighborPoint : neighborPoints){
						
						if (img.at<uchar>(neighborPoint) > 3 && labels[neighborPoint.x][neighborPoint.y] == 0){
							img.at<uchar>(neighborPoint) = 255;
							labels[neighborPoint.x][neighborPoint.y] = 1;
							pixelStack.push(neighborPoint);
						}
					}

				}


			}
		}
	}

}

vector<Point> PrincipalLineExtraction::getNeighborPoints(Point currentPixel){

	// Defining direction points
	vector<Point> directionPoints;

	directionPoints.push_back(Point( 1,  0));
	directionPoints.push_back(Point( 1, -1));
	directionPoints.push_back(Point( 0, -1));
	directionPoints.push_back(Point(-1, -1));

	directionPoints.push_back(Point(-1, 0));
	directionPoints.push_back(Point(-1, 1));
	directionPoints.push_back(Point (0, 1));
	directionPoints.push_back(Point (1, 1));

	vector<Point> neigborPoints;
	
	for (Point x : directionPoints){
		Point neigborPoint = currentPixel + x;

		if (isInsideTheBoundary(neigborPoint.x, neigborPoint.y)){
			neigborPoints.push_back(neigborPoint);
		}
	}

	return neigborPoints;
}

bool PrincipalLineExtraction::isForeground(const Mat& img, Point p){
	if (img.at<uchar>(p.x, p.y) == 255) return true;
	
	return false;
}

bool PrincipalLineExtraction::isInsideTheBoundary(int i, int j){
	if (i > 0 && i < roiHeight && j > 0 && j < roiWidth){
		return true;
	}
	else{
		return false;
	}
}

PrincipalLineExtraction::~PrincipalLineExtraction()
{
}
