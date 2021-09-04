#pragma once

#include <string_view>

#include <lexer/token.hpp>

class Lexer
{
public:
	Lexer(std::string_view data);
	void Shift();
	const Token& Peek() const;

private:
	Token Lex();
	Token LexHelper();
	std::string_view GetLine();

	std::string_view m_data;
	std::string_view m_line;
	Token m_current;
	int m_lineNumber;
};
