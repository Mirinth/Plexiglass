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
	Indent,
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
	int line;
	TokenType type;
	std::string text;
};
std::ostream& operator<<(std::ostream& os, const Token token);