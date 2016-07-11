#include "AuthenticationThread.h"

#include <QDebug>
#include <QTime>

#include <module_PalmImageReader/IPalmReader.h>
#include <module_PalmImageReader/MultPalmReader.h>
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
#include <utility/PPAException.h>
#include <utility/QtUtils.h>
#include <easylogging++.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


AuthenticationThread::AuthenticationThread(QString path, int method)
{
    this->path = path;
    this->method = method;
}

void AuthenticationThread::terminateThread(){
    isCanceled = true;
}

void AuthenticationThread::run(){

    // Timer to measure how much the authentification phase takes
    QTime authTimer;
    authTimer.start();

    int matchingScore = 0;
    int progressPercentage = 1;

    MultPalmReader* reader = new MultPalmReader();

    reader->init(path.toStdString());
    while (reader->hasNextImage() && !isCanceled){

            // Reading image and user id
            int userId = reader->readUserId();
            Mat palmImage = reader->readPalmImage();

            IPreprocessor* preprocessor = new Preprocessor();
            IRoiExtractor* roiExtractor = new SquareRoiExtractor(palmImage,preprocessor);

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

                // Extracting features from the ROI
                feature = featureExtraction->doFeatureExtraction(roi);
                feature->setUserId(userId);
                // Matching the extracted feature (1:N)
                pair<double,int> matchedResult = matcher->doMatching(feature);

                // Check if the match was correct
                if (matchedResult.second == userId){
                    matchingScore++;
                }


                //LOG(INFO) << "User " << userId << " matched to id: " << matchedResult.second;
            }catch (PPAException &e){
                LOG(INFO) << "Error during authentication: user " << userId << " Exception: " << e.what();
            }

            // Sending progress updates to the UI
            emit authPercentageComplete((progressPercentage*100/reader->getNumberOfImages()));
            progressPercentage++;
    }

    // Sending the correctly matched samples percentage to the UI
    double matchingScorePercantage = (matchingScore*100)/(reader->getNumberOfImages());

    // Getting total time
    int totalTime = authTimer.elapsed();

    // Calculating  avarage time per single match
    int avgTime = totalTime/(reader->getNumberOfImages());

    // Sending time information to the UI
    emit authMatchComplete(matchingScorePercantage);
    emit authTimeComplete(totalTime, avgTime);
}
