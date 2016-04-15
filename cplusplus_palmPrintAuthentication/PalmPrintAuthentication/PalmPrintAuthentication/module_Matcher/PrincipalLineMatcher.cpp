#include <module_Matcher\PrincipalLineMatcher.h>
#include <module_FeatureExtraction\PrincipalLineFeature.h>

PrincipalLineMatcher::PrincipalLineMatcher()
{

}


Mat PrincipalLineMatcher::doDistanceTransformation(Mat img){

	Mat invImg = Scalar::all(255) - img;


	for (int x = 1; x < invImg.rows - 1; ++x){
		for (int y = 1; y < invImg.cols - 1; ++y){

			if (invImg.at<uchar>(x, y) > invImg.at<uchar>(x - 1, y - 1) + 4) invImg.at<uchar>(x, y) = (invImg.at<uchar>(x - 1, y - 1) + 4);

			if (invImg.at<uchar>(x, y) > invImg.at<uchar>(x, y - 1) + 3) invImg.at<uchar>(x, y) = (invImg.at<uchar>(x, y - 1) + 3);

			if (invImg.at<uchar>(x, y) > invImg.at<uchar>(x + 1, y - 1) + 4) invImg.at<uchar>(x, y) = (invImg.at<uchar>(x + 1, y - 1) + 4);

			if (invImg.at<uchar>(x, y) > invImg.at<uchar>(x - 1, y) + 3) invImg.at<uchar>(x, y) = (invImg.at<uchar>(x - 1, y) + 3);
		}
	}

	for (int x = invImg.rows - 2; x >= 1; --x){
		for (int y = invImg.cols - 2; y >= 1; --y){

			if (invImg.at<uchar>(x, y) > invImg.at<uchar>(x + 1, y) + 3) invImg.at<uchar>(x, y) = (invImg.at<uchar>(x + 1, y) + 3);

			if (invImg.at<uchar>(x, y) > invImg.at<uchar>(x - 1, y + 1) + 4) invImg.at<uchar>(x, y) = (invImg.at<uchar>(x - 1, y + 1) + 4);

			if (invImg.at<uchar>(x, y) > invImg.at<uchar>(x, y + 1) + 3) invImg.at<uchar>(x, y) = (invImg.at<uchar>(x, y + 1) + 3);

			if (invImg.at<uchar>(x, y) > invImg.at<uchar>(x + 1, y + 1) + 4) invImg.at<uchar>(x, y) = (invImg.at<uchar>(x + 1, y + 1) + 4);
		}
	}

	//namedWindow("DT");
	//imshow("DT",invImg);
	//waitKey(0)	;
	return invImg;
}

int PrincipalLineMatcher::doMatching(IFeature* f){

	PrincipalLineFeature *pf = dynamic_cast<PrincipalLineFeature*> (f);
	Mat imgDT = doDistanceTransformation(pf->getImg());

	dbAdapter = new DbAdapter();
	//imwrite("dt.jpg", dtImage);
	vector<pair<int, vector<Point>>> storedFeatures = dbAdapter->getFeatures();

	int min = 10000000;
	int min_id = 0;
	int sum = 0;
	for (int i = 0; i < storedFeatures.size(); ++i){
		sum = 0;
		vector<Point> temp = storedFeatures.at(i).second;
		for (int j = 0; j < temp.size(); ++j){
			//if (j < 3) cout << (int) dtImage.at<uchar>(temp.at(j).x, temp.at(j).y);
			sum += imgDT.at<uchar>(temp.at(j).x, temp.at(j).y);
		}

		sum = sum / temp.size();
		if (sum < min){
			min = sum;
			min_id = storedFeatures.at(i).first;
		}
		
	}

	cout << "UserId: " << min_id << " Matching: " << min << endl;
	return min_id;
}


PrincipalLineMatcher::~PrincipalLineMatcher()
{
	delete dbAdapter;
}
