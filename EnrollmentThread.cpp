#include "EnrollmentThread.h"

#include <QDebug>
#include <QTime>
#include <fstream>

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
#include <utility/PPAException.h>
#include <utility/QtUtils.h>
#include <easylogging++.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

EnrollmentThread::EnrollmentThread(QString path, int method)
{
    this->path = path;
    this->method = method;
}

void EnrollmentThread::terminateThread(){
    isCanceled = true;
}

void EnrollmentThread::run(){

    // Timer to measure how much the enrollment phase takes
    QTime timer;
    timer.start();

    int progressPercentage = 1;

    MultPalmReader* reader = new MultPalmReader();
    reader->init(path.toStdString());

    while (reader->hasNextImage() && !isCanceled){

            // Reading image and user id
            int userId = reader->readUserId();
            Mat palmImage = reader->readPalmImage();

            IPreprocessor* preprocessor = new Preprocessor();
            IRoiExtractor* roiExtractor = new SquareRoiExtractor(palmImage, preprocessor);

            IFeature* feature = nullptr;
            IFeatureExtractor* featureExtraction;

            if(method == QtUtils::LINE_METHOD){
                featureExtraction = new PrincipalLineExtractor();
            }

            if(method == QtUtils::TEXTURE_METHOD){
                featureExtraction = new TextureExtractor();
            }

            try{

                // Segmenting the hand and extracting the ROI
                Mat roi = roiExtractor->doExtraction();

                // Extracting features from the ROI
                feature = featureExtraction->doFeatureExtraction(roi);

                // Saving the extracted features into database
                feature->save(userId);

            }catch (PPAException &e){
                LOG(INFO) << "Error during enrollment: user " << userId << " Exception: " << e.what();
            }

            // Sending progress updates to the UI
            emit enrollPercentageComplete((progressPercentage*100/reader->getNumberOfImages()));
            progressPercentage++;


    }

    // Getting total time
    int totalTime = timer.elapsed();

    // Calculating avarage time per a single enrollment
    int avgTime = totalTime/(reader->getNumberOfImages());

    // Sending time information to the UI
    emit enrollTimeComplete(totalTime, avgTime);
}
