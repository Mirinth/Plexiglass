#pragma once

#include <string>

enum class TokenType
{
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
	Token(TokenType type, std::string text = "");
	Token(size_t line, TokenType type, std::string text = "");
	std::string ToString() const;
	size_t line;
	TokenType type;
	std::string text;
};
