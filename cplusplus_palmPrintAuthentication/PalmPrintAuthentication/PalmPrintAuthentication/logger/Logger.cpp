#include "Logger.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void Logger::log(string tag, string message){
	string logMessage = "[" + tag + "]" + " " + message;

	cout << logMessage << endl;
	ofstream logFile("log.txt", ios_base::out | ios_base::app);
	logFile << logMessage << endl;
}
