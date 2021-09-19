#pragma once

#include <std/string>

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

class Token
{
public:
    Token();
    Token(size_t line, TokenType type, std::string text = "");
    std::string ToString() const;
    size_t line;
    TokenType type;
    std::string text;
};
