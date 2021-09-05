#pragma once

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
	Regex,
	Retry,
	Unknown,
};

class Token
{
public:
	Token();
	Token(TokenType type, std::string text = "");
	Token(size_t line, TokenType type, std::string text);
	std::string ToString() const;
	size_t line;
	TokenType type;
	std::string text;
};
