#pragma once
#include <iostream>
#include <exception>
#include <string>

class PPAException : public std::exception
{
public:
	PPAException(const std::string m = "Exception thrown") : message(m){};
	
	const char* what(){ return message.c_str(); }


private:
	std::string message;
};

