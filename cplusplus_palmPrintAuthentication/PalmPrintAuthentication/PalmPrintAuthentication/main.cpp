#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <module_PalmImageReader\IPalmReader.h>
#include <module_PalmImageReader\PalmReaderEnrollment.h>
#include <module_Preprocessing\IPreprocessing.h>
#include <module_Preprocessing\Preprocessing.h>
#include <module_RoiExtraction\IRoiExtraction.h>
#include <module_RoiExtraction\SquareRoiExtraction.h>
#include <module_RoiExtraction\IRegionSegmentation.h>
#include <module_RoiExtraction\RegionGrowing.h>
#include <module_RoiExtraction\OtsuTresholding.h>
#include <module_FeatureExtraction\IFeatureExtraction.h>
#include <module_FeatureExtraction\PrincipalLineExtraction.h>
#include <module_FeatureExtraction\IFeature.h>
#include <module_FeatureExtraction\PrincipalLineFeature.h>
#include <utility\PPAException.h>
#include <logger\Logger.h>

using namespace cv;
#define REGISTER_PALM false

int main(int argc, char *argv[]){
	
	PalmReaderEnrollment* reader = new PalmReaderEnrollment();
	reader->init("database_problems");
	
	while (reader->hasNextImage()){
		int userId = reader->readUserId();
		Mat palmImage = reader->readPalmImage();

		IPreprocessing* preprocessor = new Preprocessing();
		Mat preprocessedImage = preprocessor->doPreprocessing(palmImage);

		IRegionSegmentation* regionSegmentation = new OtsuTresholding();
		IRoiExtraction* roiExtractor = new SquareRoiExtraction(regionSegmentation);
		
		IFeature* principalLineFeature = nullptr;

		IFeatureExtraction* featureExtraction = new PrincipalLineExtraction();


		try{
			roiExtractor->doExtraction(preprocessedImage);

			principalLineFeature = featureExtraction->doFeatureExtraction(roiExtractor->getRoi());


			principalLineFeature->save(userId);

		}
		catch (PPAException &e){
			Logger::log("RoiExtraction", e.what());
		}

		delete preprocessor;
		delete regionSegmentation;
		delete roiExtractor;
		delete principalLineFeature;
		delete featureExtraction;	
	}

	delete reader;

	return 0;
}