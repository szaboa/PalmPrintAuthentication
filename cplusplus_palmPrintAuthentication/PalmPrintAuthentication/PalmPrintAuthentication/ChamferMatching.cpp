#include "ChamferMatching.h"


ChamferMatching::ChamferMatching(Mat im) : searchImage(im)
{
	/*DT*/
	Mat imgDT = doDistanceTransformation(im);
	namedWindow("DT");
	imshow("DT", imgDT);
	waitKey(0);
}

Mat ChamferMatching::doDistanceTransformation(Mat img){

	Mat invImg = Scalar::all(255) - img;
	

	for (int x = 1; x < invImg.rows-1; ++x){
		for (int y = 1; y < invImg.cols - 1; ++y){

			if (invImg.at<uchar>(x, y) > invImg.at<uchar>(x - 1, y - 1) + 4) invImg.at<uchar>(x, y) = (invImg.at<uchar>(x - 1, y - 1) + 4);

			if (invImg.at<uchar>(x, y) > invImg.at<uchar>(x, y - 1) + 3) invImg.at<uchar>(x, y) = (invImg.at<uchar>(x, y - 1) + 3);

			if (invImg.at<uchar>(x, y) > invImg.at<uchar>(x + 1, y - 1) + 4) invImg.at<uchar>(x, y) = (invImg.at<uchar>(x + 1, y - 1) + 4);

			if (invImg.at<uchar>(x, y) > invImg.at<uchar>(x - 1, y) + 3) invImg.at<uchar>(x, y) = (invImg.at<uchar>(x - 1, y) + 3);
		}
	}

	for (int x = invImg.rows - 2; x >= 1; --x){
		for (int y = invImg.cols - 2; y >= 1; --y){

			if (invImg.at<uchar>(x, y) > invImg.at<uchar>(x + 1, y) + 3) invImg.at<uchar>(x, y) = (invImg.at<uchar>(x + 1, y) + 3);

			if (invImg.at<uchar>(x, y) > invImg.at<uchar>(x - 1, y + 1) + 4) invImg.at<uchar>(x, y) = (invImg.at<uchar>(x - 1, y + 1) + 4);

			if (invImg.at<uchar>(x, y) > invImg.at<uchar>(x, y + 1) + 3) invImg.at<uchar>(x, y) = (invImg.at<uchar>(x, y + 1) + 3);

			if (invImg.at<uchar>(x, y) > invImg.at<uchar>(x + 1, y + 1) + 4) invImg.at<uchar>(x, y) = (invImg.at<uchar>(x + 1, y + 1) + 4);
		}
	}
	

	return invImg;
}

ChamferMatching::~ChamferMatching()
{
}
