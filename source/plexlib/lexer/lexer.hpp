#pragma once

#include <queue>
#include <string_view>

#include <lexer/token.hpp>

class Lexer
{
public:
	Lexer(std::string_view data);
	void Shift();
	const Token& Peek() const;

private:
	Token LexHelper(std::string_view& line);
	std::string_view GetLine();
	void LexLine();
	void FillBuffer();

	std::string_view m_data;
	std::string_view m_line;
	std::queue<Token> m_buffer;
	int m_lineNumber;
};
