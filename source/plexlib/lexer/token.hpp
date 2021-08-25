#pragma once

#include <ostream>
#include <string>

enum class TokenType
{
	Action,
	Alternator,
	End,
	Eof,
	Expression,
	Identifier,
	Keyword,
	Newline,
	Retry,
	Unknown,
};

class Token
{
public:
	Token();
	Token(size_t line, TokenType type, std::string text);
	size_t line;
	TokenType type;
	std::string text;
};
std::ostream& operator<<(std::ostream& os, const Token token);