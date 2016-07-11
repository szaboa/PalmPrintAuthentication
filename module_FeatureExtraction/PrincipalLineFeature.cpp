#include <module_FeatureExtraction/PrincipalLineFeature.h>
#include <module_DatabaseAdapter/JsonPoint.h>
#include <json/json11.hpp>
#include <utility/QtUtils.h>

using namespace json11;
using namespace cv;

PrincipalLineFeature::PrincipalLineFeature(cv::Mat img) : img(img)
{
	dbAdapter = new DbAdapter();
}

void PrincipalLineFeature::setUserId(int userId){
    this->userId = userId;
}

int PrincipalLineFeature::getUserId(){
    return userId;
}

void PrincipalLineFeature::save(int userId){
    vector<JsonPoint> jsonPoints;

    for (int i = 0; i < img.rows; ++i){
        for (int j = 0; j < img.cols; ++j){
            if (img.at<uchar>(i, j) == 255){
                jsonPoints.push_back(Point(i,j));

            }
        }
    }

    dbAdapter->insertLineFeature(userId, Json(jsonPoints).dump());
}

Mat PrincipalLineFeature::getImageRepresentation(){
	return img;
}

PrincipalLineFeature::~PrincipalLineFeature()
{
	delete dbAdapter;
}
