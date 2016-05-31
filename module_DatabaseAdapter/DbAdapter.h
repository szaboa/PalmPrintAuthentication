#pragma once

#include <sqlite3.h>
#include <sqlite_modern_cpp.h>
#include <string>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <json/json11.hpp>
#include <vector>
#include <tuple>


using namespace sqlite;
using namespace std;
using namespace cv;

/**
 * @brief The DbAdapter class is an adapter for database operations (using sqlite)
 */
class DbAdapter
{
public:
    /**
     * @brief DbAdapter
     */
	DbAdapter();

    /**
     * @brief Inserts line features into database
     * @param userId User id
     * @param jsonData Json representation of the line features
     */
	void insertLineFeature(int userId, std::string jsonData);

    /**
     * @brief Inserts texture features into database
     * @param userId User id
     * @param jsonData Json representation of the texture features
     */
	void insertTextureFeature(int userId, std::string jsonData);

    /**
     * @brief Returns the stored line features
     * @return Line features
     */
    vector <pair<int, vector<Point>> >  getLineFeatures();

    /**
     * @brief Returns the stored texture features
     * @return Texture features
     */
	vector <pair<int, json11::Json>> getTextureFeatures();
	
	~DbAdapter();

private:
    const std::string dbName = "../database/lines.db";

	database* db = nullptr;
};

