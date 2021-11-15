#pragma once

#include <exception>
#include <string>

enum class TokenType;

class PlexiException : public std::exception
{
public:
    PlexiException(const char* msg);
};

void DuplicateActionError(size_t originalLine,
                          size_t duplicateLine,
                          std::string name);
void DuplicateNameError(size_t line, size_t original, std::string name);
void MissingNameError(size_t line, std::string name);
void Error(size_t line, std::string message);
void Error(std::string expected, size_t line, TokenType type, std::string text);
