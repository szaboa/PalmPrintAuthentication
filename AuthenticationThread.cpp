#include <QDebug>
#include <QTime>

#include "AuthenticationThread.h"

#include <module_PalmImageReader/IPalmReader.h>
#include <module_PalmImageReader/PalmReaderEnrollment.h>
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
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


#include <utility/PPAException.h>
#include <utility/QtUtils.h>
#include <easylogging++.h>

AuthenticationThread::AuthenticationThread(QString path, int method)
{
    this->path = path;
    this->method = method;
}

void AuthenticationThread::run(){
    QTime timer;
    timer.start();

    int score = 0;
    int percentage = 1;
    PalmReaderEnrollment* reader = new PalmReaderEnrollment();

    reader->init(path.toStdString());

    while (reader->hasNextImage()){
            int userId = reader->readUserId();
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
                feature = featureExtraction->doFeatureExtraction(roiExtractor->getRoi());
                pair<double,int> matchedResult = matcher->doMatching(feature);

                if (matchedResult.second == userId){
                    score++;
                }

                LOG(INFO) << "User " << userId << " matched to id: " << matchedResult.second;
            }catch (PPAException &e){
                LOG(INFO) << "User " << userId << " Exception: " << e.what();
            }

            emit authPercentageComplete((percentage*100/reader->getNumberOfImages()));
            percentage++;
    }

    double matchPercantage = (score*100)/(reader->getNumberOfImages());
    LOG(INFO) << "SCORE: " << matchPercantage;
    //gDebug() << matchPercantage;
    int totalTime = timer.elapsed();
    int avgTime = totalTime/(reader->getNumberOfImages());

    emit authMatchComplete(matchPercantage);
    emit authTimeComplete(totalTime, avgTime);
}
