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


		*db <<
			"create table if not exists texture ("
			"   _id integer primary key autoincrement not null,"
			"   userId int,"
			"   featureData text"
			");";

	}
	catch (std::exception& e) {
		Logger::log(TAG,e.what());
	}
}

void DbAdapter::insertTextureFeature(int userId, std::string jsonData){

	try{
		*db << "insert into texture (userId, featureData) values (?,?);"
			<< userId
			<< jsonData;
		Logger::log(TAG, "User id: " + std::to_string(userId) + " recorded in database");
	}
	catch (std::exception& e) {
		Logger::log(TAG, e.what());
	}

}

void DbAdapter::insertLineFeature(int userId, std::string jsonData){

	try{
		*db  << "insert into palm_lines (userId, featureData) values (?,?);"
			 << userId
			 << jsonData;
		Logger::log(TAG, "User id: " + std::to_string(userId) + " recorded in database");
	}
	catch (std::exception& e) {
		Logger::log(TAG, e.what());
	}
	
}

vector < pair<int, vector<Point>>>  DbAdapter::getFeatures(){
	vector <pair<int, vector<Point>>> records;
	try{
		*db << "select userId, featureData from palm_lines;"
			>> [&](int userId, std::string featureData){
			std::string error;
			Json features = Json::parse(featureData, error);

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
	}
	catch (std::exception& e) {
		Logger::log(TAG, e.what());
	}

	return records;

}

vector <pair<int, Json>> DbAdapter::getTextureFeatures(){
	vector <pair<int, Json>> records;

	try{
	*db << "select userId, featureData from texture;"
		>> [&](int userId, std::string featureData){

			std::string error;
			Json feature = Json::parse(featureData, error);

			if (feature.is_null()) {
				Logger::log(TAG, error);
			}
			else{
				records.push_back(make_pair(userId, feature));
			}
		};
	}
	catch (std::exception& e) {
		Logger::log(TAG, e.what());
	}

	return records;
}

DbAdapter::~DbAdapter()
{

	delete db;

}
