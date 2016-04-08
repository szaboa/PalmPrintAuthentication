#include <module_DatabaseAdapter\DbAdapter.h>
#include <string>
#include <iostream>
#include <logger\Logger.h>
#include <json/json11.hpp>
#include <module_DatabaseAdapter\JsonPoint.h>

using namespace sqlite;
using namespace json11;

DbAdapter::DbAdapter(){
	try{
		db = new database(dbName);
		
		*db <<
			"create table if not exists palm_lines ("
			"   _id integer primary key autoincrement not null,"
			"   userId int,"
			"   featureData text"
			");";

	}
	catch (std::exception& e) {
		Logger::log(TAG,e.what());
	}
}

void DbAdapter::insertFeature(int userId, std::string jsonData){

	*db  << "insert into palm_lines (userId, featureData) values (?,?);"
		<< userId
		<< jsonData;

	Logger::log(TAG, "User id: " + std::to_string(userId) + " recorded in database");
}

vector < pair<int, vector<Point>>>  DbAdapter::getFeatures(){
	vector < pair<int, vector<Point>>> records;

	*db << "select userId, featureData from palm_lines;"
		>> [&](int userId, std::string featureData){
			std::string error;
			Json features = Json::parse(featureData,error);
	
			if (features.is_null()) {
				Logger::log(TAG, error);
			}
			else{
				
				vector<Point> featurePoints;
				Json::array a = features.array_items();
				
				for (auto it = a.begin(); it < a.end(); ++it){
					Json::array b = it->array_items();
					Point p(b.at(0).int_value(), b.at(1).int_value());
					featurePoints.push_back(p);
				}

				records.push_back(make_pair(userId, featurePoints));
			}
		};

	return records;

}

DbAdapter::~DbAdapter()
{

	delete db;

}
