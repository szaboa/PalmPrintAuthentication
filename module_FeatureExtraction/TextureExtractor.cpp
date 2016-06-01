#include <module_FeatureExtraction/TextureExtractor.h>
#include <module_FeatureExtraction/TextureFeature.h>
#include <iostream>
#include <math.h>
#include <fstream>

using namespace std;

#define pi 3.1415

TextureExtractor::TextureExtractor()
{
}

pair<Mat,Mat> TextureExtractor::create2dGaborFitler(int kSize, double theta, double u, double sigma){

	Mat imMat(Size(kSize,kSize), CV_32FC1);
	Mat reMat(Size(kSize, kSize), CV_32FC1);

	int xmax = floor(kSize / 2);
	int ymax = floor(kSize / 2);

	int xmin = -xmax;
	int ymin = -ymax;
	
	for (int x = xmin; x <= xmax; ++x){
		for (int y = ymin; y <= ymax; ++y){
			double im = 1 / (2 * pi*sigma*sigma)*exp(-(x*x + y*y) / (2 * sigma*sigma)) * sin(2 * pi*(u*x*cos(theta) + u*y*sin(theta)));
			double re = 1 / (2 * pi*sigma*sigma)*exp(-(x*x + y*y) / (2 * sigma*sigma)) * cos(2 * pi*(u*x*cos(theta) + u*y*sin(theta)));
			///cout << "("  << xmax-x << "," << y  ) << "im << endl;
			imMat.at<float>(xmax - x, ymax - y) = im;
			reMat.at<float>(xmax - x, ymax - y) = re;
		}
	}

	
    // Tuning DC offset to 0
	double d = sum(reMat)[0] / (pow((2 * kSize + 1),2));
	reMat = reMat - d;
	
	return make_pair(reMat, imMat);
}

Mat TextureExtractor::preprocessing(Mat roi){
    Mat grayscale(roi.size(),CV_8UC1);
    cvtColor(roi, grayscale, CV_RGB2GRAY);

    resize(grayscale, grayscale, Size(128, 128));
    return grayscale;
}

IFeature* TextureExtractor::doFeatureExtraction(Mat roi){

	Mat reMat, imMat, filtReMat, filtImMat;
    Mat grayscale = preprocessing(roi);
    // Creating 2D Gabor filter
    auto filters = create2dGaborFitler(35, 90, 0.0916, 5.6179);
	
    // Convolution with the Gabor filter
	filter2D(grayscale, filtReMat, -1, filters.first,  Point(-1, -1), 0, BORDER_DEFAULT);
	filter2D(grayscale, filtImMat, -1, filters.second, Point(-1, -1), 0, BORDER_DEFAULT);
	
	threshold(filtReMat, filtReMat, 0, 255, CV_THRESH_BINARY);
	threshold(filtImMat, filtImMat, 0, 255, CV_THRESH_BINARY);

	IFeature* textureFeature = new TextureFeature(filtReMat, filtImMat);

	return textureFeature;
}

TextureExtractor::~TextureExtractor()
{
}
