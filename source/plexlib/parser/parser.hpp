#pragma once

#include <exception>
#include <string_view>

#include <parser/tree.hpp>

class ParseException : public std::exception
{
public:
	ParseException(const char* msg);
};

FileNode Parse(std::string_view data);
