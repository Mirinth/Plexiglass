#pragma once

#include <exception>

#include <parser/tree.hpp>

class SemanticException : public std::exception
{
public:
	SemanticException(const char* msg);
};

void Analyze(FileNode file);
