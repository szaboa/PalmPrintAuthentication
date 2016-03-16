#pragma once

#include <sqlite3.h>
#include <sqlite_modern_cpp.h>
#include <string>

using namespace sqlite;

class DbAdapter
{
public:
	DbAdapter();

	void insertFeature(int userId, std::string jsonData);
	
	~DbAdapter();

private:
	const std::string TAG = "[DbAdapter]";

	const std::string dbName = "database/lines.db";

	database* db;
};

