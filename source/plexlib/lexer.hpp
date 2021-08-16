#pragma once

#include <ostream>
#include <string>
#include <string_view>

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

class Lexer
{
public:
	Lexer(std::string_view data);
	void Shift();
	const Token& Current() const;
	const Token& Next() const;

private:
	void Lex();

	std::string_view m_data;
	Token m_current;
	Token m_next;
	int m_line;
	int m_state;
	bool m_startOfLine;
};
