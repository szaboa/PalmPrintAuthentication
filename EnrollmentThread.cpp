#include <QDebug>
#include <QTime>

#include "EnrollmentThread.h"

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

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


#include <utility/PPAException.h>
#include <utility/QtUtils.h>
#include <easylogging++.h>
#include <fstream>

using namespace cv;

EnrollmentThread::EnrollmentThread(QString path, int method)
{
    this->path = path;
    this->method = method;
}

void EnrollmentThread::run(){
    QTime timer;
    timer.start();

    int percentage = 0;
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
            IFeatureExtraction* featureExtraction;

            if(method == QtUtils::LINE_METHOD){
                featureExtraction = new PrincipalLineExtraction();
            }

            if(method == QtUtils::TEXTURE_METHOD){
                featureExtraction = new TextureExtraction();
            }

            try{
                roiExtractor->doExtraction(preprocessedImage);
                feature = featureExtraction->doFeatureExtraction(roiExtractor->getRoi());
                feature->save(userId);

                LOG(INFO) << "User " << userId << " successfuly recorded in database.";
            }catch (PPAException &e){
                LOG(INFO) << "User " << userId << " Exception: " << e.what();
            }


            emit enrollPercentageComplete((percentage*100/reader->getNumberOfImages())+1);
            percentage++;


    }

    int totalTime = timer.elapsed();
    int avgTime = totalTime/(reader->getNumberOfImages());

    emit enrollTimeComplete(totalTime, avgTime);
}
