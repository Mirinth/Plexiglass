#pragma once

#include <string_view>

class ParseException : public std::exception
{
public:
	ParseException(const char* msg);
};

void Parse(std::string_view data);
