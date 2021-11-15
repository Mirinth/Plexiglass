#pragma once

#include <string>

enum class TokenType
{
    Alternator,
    Eof,
    Indent,
    Keyword,
    Regex,
    Text,
    Unknown,
};

std::string ToString(TokenType type, const std::string& text);
