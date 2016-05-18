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

    Mat img;

	Mat histChanel1, histChanel2, histChanel3;
	
	// number of bins
	int histSize = 256;


	/*Min and max color values in the first, second and thrid chanel */
	int minValueChanel1, minValueChanel2, minValueChanel3;
	int maxValueChanel1, maxValueChanel2, maxValueChanel3;


public:

	
	Histogram(Mat img);
	

	int getMinValueChanel1() const;
	int getMaxValueChanel1() const;

	int getMinValueChanel2() const;
	int getMaxValueChanel2() const;

	int getMinValueChanel3() const;
	int getMaxValueChanel3() const;


	/*Calculate and display the histogram*/
	void calculateHistogram();
	void drawHistogram();

	~Histogram();
	
};

