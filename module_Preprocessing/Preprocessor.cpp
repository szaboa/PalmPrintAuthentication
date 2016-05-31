#include <module_Preprocessing/Preprocessor.h>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <math.h>

using namespace cv;
using namespace boost::numeric::ublas;
using namespace std;

Preprocessor::Preprocessor()
{
}

Mat Preprocessor::doPreprocessing(cv::Mat img){

    Mat rgbImg = img.clone();
    Mat ycbcrImg;

    // Convert original image to YCbCr color space
    cvtColor(rgbImg, ycbcrImg, CV_RGB2YCrCb);

    // Parameters of the skin-color modell
    double c11 = 0.0479;
    double c12 = 0.0259;
    double c21 = 0.0259;
    double c22 = 0.0212;

    double k1 = 0.0;
    double k2 = 0.0;
    double x1, x2;
    double m1 = 113.9454;
    double m2 = 157.5052;
    double f1, f2;


    Mat thresholded(ycbcrImg.size(), CV_8UC1);
    thresholded.setTo(Scalar(0));

    //Segmenting the hand
    double p = 0;

    for (int i = 0; i < ycbcrImg.rows; ++i){
        for (int j = 0; j < ycbcrImg.cols; ++j){

            x1 = (int)ycbcrImg.at<Vec3b>(i, j)[1];
            x2 = (int)ycbcrImg.at<Vec3b>(i, j)[2];

                f1 = -0.5*(x1 - m1);
                f2 = -0.5*(x2 - m2);
                k1 = f1*c11 + f2*c21;
                k2 = f1*c12 + f2*c22;

                // Probability of the pixel that belongs to a skin region
                p = exp(k1*(x1 - m1) + k2*(x2 - m2));


                if (p > 0.15){
                    thresholded.at<uchar>(i, j) = 255;
                }
        }
    }

    return thresholded;
}


Preprocessor::~Preprocessor()
{
}
