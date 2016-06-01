#include <module_Matcher/TextureMatcher.h>
#include <module_FeatureExtraction/TextureFeature.h>
#include <module_FeatureExtraction/IFeature.h>
#include <json/json11.hpp>
#include <iostream>
#include <vector>
#include <bitset>
#include <string>
#include <easylogging++.h>
#include <utility/PPAException.h>

using namespace std;
using namespace json11;
using namespace cv;

TextureMatcher::TextureMatcher()
{

}

Mat TextureMatcher::decode(vector<int> values){
	Mat img(Size(128, 128), CV_8UC1);
	img.setTo(Scalar(0));
	int cnt = 0;

	for (int x = 0; x < img.cols; ++x){
		for (int y = 0; y < img.rows; y += 8){
			string s = bitset<8>(values.at(cnt)).to_string();
			cnt++;
			for (int k = 0; k < 8; ++k){
				if (s.at(k) == '1'){
					img.at<uchar>(x,y+k) = 255;
				}
			}
		}
	}

	return img;
}

pair<double,int> TextureMatcher::doMatching(IFeature* f){

	dbAdapter = new DbAdapter();

	TextureFeature *tf = dynamic_cast<TextureFeature*> (f);
	Mat searchReMat, searchImMat;

	searchReMat = tf->getRealComponent();
	searchImMat = tf->getImaginaryComponent();

    Mat reMat, imMat, matchedReMat, matchedImMat;

	vector<int> reVec;
	vector<int> imVec;

    double minScore = INT_MAX;
	int minUserId;

    // Get the stored features
	vector<pair<int,Json>> records = dbAdapter->getTextureFeatures();

    if(records.size() == 0){
        throw PPAException("Error: database is empty");
    }

    // Iterate through the stored features and calculate the distance
	for (auto it = records.begin(); it < records.end(); ++it){
		

		int userId = it->first;
		Json data = it->second;
		Json::array realPart = data["realPart"].array_items();
		Json::array imaginaryPart = data["imaginaryPart"].array_items();

		for (auto i = realPart.begin(); i < realPart.end(); ++i){
            reVec.push_back(i->int_value());
		}

		for (auto i = imaginaryPart.begin(); i < imaginaryPart.end(); ++i){
			imVec.push_back(i->int_value());
		}

        reMat = decode(reVec);
        imMat = decode(imVec);


		reVec.clear();
		imVec.clear();

        // Calculate translation invariant Hamming distance

        int S = 2;
		int T = 2;

		int sum = 0;
		for (int s = -2; s < S; ++s){
			for (int t = -2; t < T; ++t){
				
				sum = 0;
				for (int x = 0; x < searchReMat.rows; ++x){
					for (int y = 0; y < searchReMat.cols; ++y){

						if ((x + s) >= 0 && (x + s) < reMat.cols && (y + t) >= 0 && (y + t) < reMat.rows){
							if (reMat.at<uchar>(x, y) != searchReMat.at<uchar>(x + s, y + t)) sum++;
							if (imMat.at<uchar>(x, y) != searchImMat.at<uchar>(x + s, y + t)) sum++;
						}
                    }
				}

                double d = sum / pow(2 * searchImMat.rows, 2);

                if (d < minScore){
                    minScore = d;
					minUserId = userId;
                    matchedReMat = reMat;
                    matchedImMat = imMat;
				}
			}
		}
	}

    matchedFeature = new TextureFeature(matchedReMat,matchedImMat);

    return make_pair(minScore,minUserId);
}

IFeature* TextureMatcher::getMatchedFeature(){
    return matchedFeature;
}



TextureMatcher::~TextureMatcher()
{
	delete dbAdapter;
    delete matchedFeature;
}
