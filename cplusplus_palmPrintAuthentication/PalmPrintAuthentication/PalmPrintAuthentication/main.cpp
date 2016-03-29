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
#include <module_Matcher\IMatcher.h>
#include <module_Matcher\PrincipalLineMatcher.h>
#include <utility\PPAException.h>
#include <logger\Logger.h>
#include <fstream>



using namespace cv;
#define REGISTER_PALM false



bool isBlack(Mat &img, int i, int j){
	return (img.at<Vec3b>(i, j)[0] <= 10 || img.at<Vec3b>(i, j)[1] <= 10 || img.at<Vec3b>(i, j)[2] <= 10);
}


int main(int argc, char *argv[]){
	
	int score = 0;


	PalmReaderEnrollment* reader = new PalmReaderEnrollment();
	reader->init("database_test");
	
	while (reader->hasNextImage()){
		int userId = reader->readUserId();
		Mat palmImage = reader->readPalmImage();
		

		IPreprocessing* preprocessor = new Preprocessing();
		Mat preprocessedImage = preprocessor->doPreprocessing(palmImage);

		IRegionSegmentation* regionSegmentation = new OtsuTresholding();
		IRoiExtraction* roiExtractor = new SquareRoiExtraction(regionSegmentation);
		
		IFeature* principalLineFeature = nullptr;
		
		IFeatureExtraction* featureExtraction = new PrincipalLineExtraction();
		IMatcher* chamferMatcher = new PrincipalLineMatcher();

		try{
			roiExtractor->doExtraction(preprocessedImage);

			principalLineFeature = featureExtraction->doFeatureExtraction(roiExtractor->getRoi());

			if (REGISTER_PALM){
				principalLineFeature->save(userId);
			}
			else{
				PrincipalLineFeature *pf = dynamic_cast<PrincipalLineFeature*> (principalLineFeature);
				

				int matchedId = chamferMatcher->doMatching(pf->getImg());

				if (matchedId == userId){
					score++;
				}
			}
			
		}
		catch (PPAException &e){
			Logger::log("RoiExtraction", e.what());
		}
	
		delete chamferMatcher;
		delete preprocessor;
		delete regionSegmentation;
		delete roiExtractor;
		delete principalLineFeature;
		delete featureExtraction;	
	
	}

	cout << endl << "SCORE : " << score << endl;
	
	delete reader;

	return 0;
}