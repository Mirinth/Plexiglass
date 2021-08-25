#include <parser/parser.hpp>

ParseException::ParseException(const char* msg)
	: std::exception(msg)
{}

void Parse(std::string_view /*data*/)
{
	throw ParseException("Not implemented");
}
