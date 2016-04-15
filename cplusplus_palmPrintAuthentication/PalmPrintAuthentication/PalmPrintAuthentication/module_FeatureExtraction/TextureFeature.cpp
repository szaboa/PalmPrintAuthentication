#include <module_FeatureExtraction\TextureFeature.h>
#include <iostream>

using namespace cv;
using namespace std;
using namespace json11;

TextureFeature::TextureFeature(Mat reMat, Mat imMat) : reMat(reMat), imMat(imMat)
{
	dbAdapter = new DbAdapter();
	
}

unsigned char TextureFeature::bitsToByte(string bits){
	unsigned char byte = 0;

	for (int i = 0; i < 8; ++i){
		
		byte <<= 1;
		if (bits[i] == '1'){
			byte |= 1;
		}
		
	}

	return byte;
}

void TextureFeature::save(int userId){
	Json::array realPart, imaginaryPart;

	for (int x = 0; x < reMat.rows; ++x){
		for (int y = 0; y < reMat.cols; y += 8){
			string si, sr;
			for (int k = y; k < y + 8; ++k){
				if (reMat.at<uchar>(x, k) == 255){
					sr.append("1");
				}
				else{
					sr.append("0");
				}

				if (imMat.at<uchar>(x, k) == 255){
					si.append("1");
				}
				else{
					si.append("0");
				}



			}
			imaginaryPart.push_back((int)bitsToByte(si));
			realPart.push_back((int)bitsToByte(sr));
			si = ""; sr = "";

		}
	}
	
	Json feature = Json::object{
		{ "realPart", realPart },
		{ "imaginaryPart", imaginaryPart }
		
	};

	dbAdapter->insertTextureFeature(userId, feature.dump());
}

Mat TextureFeature::getRealComponent(){
	return reMat;
}

Mat TextureFeature::getImaginaryComponent(){
	return imMat;
}

TextureFeature::~TextureFeature()
{
	delete dbAdapter;
}
