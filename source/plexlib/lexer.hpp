#pragma once

#include <string>
#include <string_view>

#include "token.hpp"

enum class LexerState;

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
	LexerState m_state;
	bool m_startOfLine;
};
