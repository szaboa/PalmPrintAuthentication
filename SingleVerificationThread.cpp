#include "SingleVerificationThread.h"

#include <module_PalmImageReader/IPalmReader.h>
#include <module_PalmImageReader/PalmReaderMatching.h>
#include <module_Preprocessing/IPreprocessing.h>
#include <module_Preprocessing/Preprocessing.h>
#include <module_RoiExtraction/IRegionSegmentation.h>
#include <module_RoiExtraction/IRoiExtraction.h>
#include <module_RoiExtraction/SkinModelSegmentation.h>
#include <module_RoiExtraction/RegionGrowing.h>
#include <module_RoiExtraction/SquareRoiExtraction.h>
#include <module_FeatureExtraction/IFeature.h>
#include <module_FeatureExtraction/IFeatureExtraction.h>
#include <module_FeatureExtraction/PrincipalLineExtraction.h>
#include <module_FeatureExtraction/PrincipalLineFeature.h>
#include <module_FeatureExtraction/TextureExtraction.h>
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

    PalmReaderMatching* reader = new PalmReaderMatching();

    reader->init(path.toStdString());

    int userId = reader->readUserId();
    emit sendQueryImageId(userId);
    Mat palmImage = reader->readPalmImage();

    IPreprocessing* preprocessor = new Preprocessing();
    Mat preprocessedImage = preprocessor->doPreprocessing(palmImage);

    IRegionSegmentation* regionSegmentation = new SkinModelSegmentation();
    IRoiExtraction* roiExtractor = new SquareRoiExtraction(regionSegmentation);
    IFeature* feature = nullptr;
    IFeatureExtraction* featureExtraction = nullptr;
    IMatcher* matcher  = nullptr;

    if(method == QtUtils::LINE_METHOD){
        featureExtraction = new PrincipalLineExtraction();
        matcher = new PrincipalLineMatcher();
    }

    if(method == QtUtils::TEXTURE_METHOD){
        featureExtraction = new TextureExtraction();
        matcher = new TextureMatcher();
    }


    LOG(INFO) << "Start matching phase for user id: " << userId;

    try{
        roiExtractor->doExtraction(preprocessedImage);
        emit sendExtractedRoi(roiExtractor->getRoi());
        feature = featureExtraction->doFeatureExtraction(roiExtractor->getRoi());

        emit sendExtractedFeature(feature->getFeature());

        pair<double,int> matchingResult = matcher->doMatching(feature);
        IFeature* matchedFeature = matcher->getMatchedFeature();
        emit sendMatchedFeature(matchingResult.first, matchingResult.second, matchedFeature->getFeature());

        LOG(INFO) << "User " << userId << " matched to id: " << matchingResult.second;
    }catch (PPAException &e){
        LOG(INFO) << "User " << userId << " Exception: " << e.what();
    }
}
