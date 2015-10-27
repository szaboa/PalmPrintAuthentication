#pragma once


#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;

/*
* This singleton class calculates and displays the histogram of the given image
*/
class Histogram
{

private:
	/*private constructor*/
	Histogram(){}; 
	/*private copy constructor*/
	Histogram(Histogram const&){};
	/*private assignment operator*/
	Histogram& operator = (Histogram const&){};

	static Histogram* mInstance;

	/*Input image, the histogram will be calculated based on this image*/
	static Mat img;

	/*Min and max color values in the first, second and thrid chanel */
	static int minValueChanel1, minValueChanel2, minValueChanel3;
	static int maxValueChanel1, maxValueChanel2, maxValueChanel3;

	/*Calculate and display the histogram*/
	static void calculateHistogram();

public:
	
	static Histogram* getInstance();

	/*Init img object, and call calculateHistogram method*/
	static void init(Mat img);
	static int getMinValueChanel1();
	static int getMaxValueChanel1();

	static int getMinValueChanel2();
	static int getMaxValueChanel2();

	static int getMinValueChanel3();
	static int getMaxValueChanel3();
	
};

