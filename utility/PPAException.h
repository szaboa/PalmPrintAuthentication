#pragma once
#include <iostream>
#include <exception>
#include <string>

/**
 * @brief The PPAException class is an own exception class, used application-wide
 */
class PPAException : public std::exception
{
public:
    /**
     * @brief PPAException
     * @param m Message
     */
	PPAException(const std::string m = "Exception thrown") : message(m){};
	
    /**
     * @brief Returns the exception message
     * @return Message
     */
	const char* what(){ return message.c_str(); }


private:
	std::string message;
};

