#include "SingleVerificationThread.h"

#include <module_PalmImageReader/IPalmReader.h>
#include <module_PalmImageReader/SinglePalmReader.h>
#include <module_Preprocessing/IPreprocessor.h>
#include <module_Preprocessing/Preprocessor.h>
#include <module_RoiExtraction/IRoiExtractor.h>
#include <module_RoiExtraction/SquareRoiExtractor.h>
#include <module_FeatureExtraction/IFeature.h>
#include <module_FeatureExtraction/IFeatureExtractor.h>
#include <module_FeatureExtraction/PrincipalLineExtractor.h>
#include <module_FeatureExtraction/PrincipalLineFeature.h>
#include <module_FeatureExtraction/TextureExtractor.h>
#include <module_FeatureExtraction/TextureFeature.h>
#include <module_Matcher/IMatcher.h>
#include <module_Matcher/PrincipalLineMatcher.h>
#include <module_Matcher/TextureMatcher.h>
#include "utility/QtUtils.h"
#include "utility/PPAException.h"
#include "easylogging++.h"

using namespace cv;

SingleVerificationThread::SingleVerificationThread(QString path, int method)
{
    this->path = path;
    this->method = method;
}

void SingleVerificationThread::run(){

    SinglePalmReader* reader = new SinglePalmReader();

    reader->init(path.toStdString());

    // Reading image and user id
    int userId = reader->readUserId();
    Mat palmImage = reader->readPalmImage();

    // Sending user id to the UI
    emit sendQueryImageId(userId);

    IPreprocessor* preprocessor = new Preprocessor();
    IRoiExtractor* roiExtractor = new SquareRoiExtractor(palmImage, preprocessor);

    IMatcher* matcher  = nullptr;
    IFeature* feature = nullptr;
    IFeatureExtractor* featureExtraction = nullptr;

    if(method == QtUtils::LINE_METHOD){
        featureExtraction = new PrincipalLineExtractor();
        matcher = new PrincipalLineMatcher();
    }

    if(method == QtUtils::TEXTURE_METHOD){
        featureExtraction = new TextureExtractor();
        matcher = new TextureMatcher();
    }

    try{
        // Segmenting the hand and extracting the ROI
        Mat roi = roiExtractor->doExtraction();

        // Sending the extracted roi to the UI
        emit sendExtractedRoi(roi);

        // Extracting features from the ROI
        feature = featureExtraction->doFeatureExtraction(roi);

        // Sending the extracted features to the UI
        emit sendExtractedFeature(feature->getImageRepresentation());

        // Matching the extracted feature (1:N)
        pair<double,int> matchingResult = matcher->doMatching(feature);

        IFeature* matchedFeature = matcher->getMatchedFeature();

        // Sending the matched feature, matched user id and distance to the UI
        emit sendMatchedFeature(matchingResult.first, matchingResult.second, matchedFeature->getImageRepresentation());
    }catch (PPAException &e){
        LOG(INFO) << "Error during single verification: user " << userId << " Exception: " << e.what();
    }
}
