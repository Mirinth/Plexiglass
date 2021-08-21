#pragma once

#include <string>
#include <string_view>

#include <lexer/rules.hpp>
#include <lexer/token.hpp>

class Lexer
{
public:
	Lexer(std::string_view data);
	void Shift();
	const Token& Current() const;
	const Token& Next() const;

private:
	Token Lex();
	Token LexHelper();

	std::string_view m_data;
	Token m_current;
	Token m_next;
	int m_line;
	State m_state;
	bool m_startOfLine;
};
