#pragma once

#include <exception>
#include <string>

class Token;

class PlexiException : public std::exception
{
public:
	PlexiException(const char* msg);
};

void DuplicateNameError(size_t line, size_t original, std::string name);
void Error(size_t line, std::string message);
void Error(std::string expected, const Token found);
