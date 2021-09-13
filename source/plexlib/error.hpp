#pragma once

#include <exception>
#include <string>

class Token;

class ParseException : public std::exception
{
public:
	ParseException(const char* msg);
};

void Error(size_t line, std::string message);
void Error(std::string expected, const Token found);