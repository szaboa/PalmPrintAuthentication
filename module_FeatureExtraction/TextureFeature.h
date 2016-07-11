#pragma once
#include <module_FeatureExtraction/IFeature.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <module_DatabaseAdapter/DbAdapter.h>
#include <string>
#include <json/json11.hpp>

/**
 * @brief The TextureFeature class is implementation of the IFeature, represents a particular, texture-based biometric feature
 */
class TextureFeature : public IFeature
{

public:

    /**
     * @brief Constructor
     * @param realMat Real part of the extracted texture-feature
     * @param imaginaryMat Imaginary part of the extracted texture-feature
     */
	TextureFeature(cv::Mat realMat, cv::Mat imaginaryMat);

    /**
     * @brief Saves the feature into databaes
     * @param userId The feature belongs to this user id
     */
	void save(int userId) override;

    /**
     * @brief Returns the image representation of the feature
     * @return Image that contains the feature
     */
    Mat getImageRepresentation() override;

    /**
     * @brief Returns the real part of the feature
     * @return Real part image
     */
	Mat getRealComponent();

    /**
     * @brief Returns the imaginary part of the feature
     * @return Imaginary part image
     */
	Mat getImaginaryComponent();

    int getUserId() override;
    void setUserId(int) override;

	~TextureFeature();
private:
	DbAdapter *dbAdapter = nullptr;

    /**
     * @brief Converts a string representation of 8 bits to a byte
     * @param A string representation of the 8 bits
     * @return Converted byte
     */
	unsigned char bitsToByte(std::string bits);

	Mat reMat, imMat;
    int userId;
};

