#pragma once
#include <string>
class Logger
{
public:
	static void log(std::string tag, std::string message);
private:
	/*private constructor*/
	Logger(){};

	/*private copy constructor*/
	Logger(Logger const&){};

	/*private assignment operator*/
	Logger& operator = (Logger const&){};
};

