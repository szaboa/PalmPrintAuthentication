#include "Logger.h"
#include <iostream>

using namespace std;

void Logger::log(string tag, string message){
	cout << "[" << tag << "] " << message << endl;
}
