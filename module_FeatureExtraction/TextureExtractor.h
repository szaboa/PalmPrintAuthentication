#pragma once
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <module_FeatureExtraction/IFeatureExtractor.h>
#include <module_FeatureExtraction/IFeature.h>

using namespace cv;

/**
 * @brief The TextureExtractor class is an implementation of the IFeatureExtractor, extracts texture features based on the ROI
 */
class TextureExtractor :public IFeatureExtractor
{
public:
    /**
     * @brief TextureExtractor
     */
    TextureExtractor();

    /**
     * @brief Extracts the texture features, based on the ROI
     * @param ROI of the palm image
     * @return IFeature implementation that contains the extracted features
     */
	IFeature* doFeatureExtraction(Mat roi) override;

    ~TextureExtractor();

private:
    /**
     * @brief Creates a 2D Gabor filter with the given parameters
     * @param kSize Kernel size
     * @param theta Orientation of the sinusoidal plane wave
     * @param u Frequency of the sinusoidal plane wave
     * @param sigma Deviation of the Gauss kernel function
     * @return Real part and imaginary part of the kernel
     */
    std::pair<Mat,Mat> create2dGaborFitler(int kSize, double theta, double u, double sigma);
	

};

