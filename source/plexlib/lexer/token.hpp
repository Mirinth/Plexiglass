#pragma once

#include <ostream>
#include <string>

enum class TokenType
{
	ActDec,
	ActInc,
	ActProduce,
	ActProduceNil,
	ActRewind,
	ActTransition,
	Alternator,
	Eof,
	Identifier,
	KwdExpression,
	KwdPattern,
	KwdRule,
	Newline,
	Regex,
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