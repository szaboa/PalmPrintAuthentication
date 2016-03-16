#include "DbAdapter.h"
#include <string>
#include <iostream>
#include "Logger.h"

using namespace sqlite;

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
		std::cout << e.what() << std::endl;
	}
}

void DbAdapter::insertFeature(int userId, std::string jsonData){

	*db  << "insert into palm_lines (userId, featureData) values (?,?);"
		<< userId
		<< jsonData;

	Logger::log(TAG, "New record got assigned id" + db->last_insert_rowid());
}

DbAdapter::~DbAdapter()
{
	delete db;
}
