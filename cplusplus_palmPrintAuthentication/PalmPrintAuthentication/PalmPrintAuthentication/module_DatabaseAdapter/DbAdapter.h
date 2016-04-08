#pragma once

#include <sqlite3.h>
#include <sqlite_modern_cpp.h>
#include <string>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <vector>
#include <tuple>


using namespace sqlite;
using namespace std;
using namespace cv;

class DbAdapter
{
public:
	DbAdapter();

	void insertFeature(int userId, std::string jsonData);
	vector <pair<int, vector<Point>> >  getFeatures();
	
	~DbAdapter();

private:
	const std::string TAG = "DbAdapter";

	const std::string dbName = "database/lines.db";

	database* db = nullptr;
};

