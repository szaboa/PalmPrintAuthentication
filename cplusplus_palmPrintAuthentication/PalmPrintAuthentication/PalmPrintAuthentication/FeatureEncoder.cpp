#include "FeatureEncoder.h"
#include "DbAdapter.h"
#include <vector>
#include "json/json11.hpp"
#include "JsonPoint.h"
#include <iostream>

using namespace json11;
using namespace std;

FeatureEncoder::FeatureEncoder()
{
	dbAdapter = new DbAdapter();
}

void FeatureEncoder::encodeAndSave(int userId, const Mat &img, vector<vector<Point>> lineComponents){
	

	vector<JsonPoint> jsonPoints;

	for (int i = 0; i < lineComponents.size(); ++i){
		for (int j = 0; j < lineComponents[i].size(); ++j){
			jsonPoints.push_back(lineComponents[i].at(j));
		}
	}

	//cout << Json(jsonPoints).dump();
	//std::string json_str = my_json.dump();

	dbAdapter->insertFeature(userId, Json(jsonPoints).dump());

	namedWindow("After thinning", WINDOW_AUTOSIZE);
	imshow("After thinning", img);
	imwrite("output.jpg", img);
	waitKey(0);
}

FeatureEncoder::~FeatureEncoder()
{
	delete dbAdapter;
}
