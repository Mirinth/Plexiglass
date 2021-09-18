#pragma once

#include <std/queue>
#include <std/string>
#include <string_view>

#include <lexer/token.hpp>

class Lexer
{
public:
	Lexer(std::string_view data);
	void Shift();
	const Token& Peek() const;

private:
	std::string LexToken(std::string_view& line);
	std::string_view GetLine();
	void LexLine(std::string_view line);
	void FillBuffer();

	std::string_view m_data;
	std::queue<Token> m_buffer;
	size_t m_lineNumber;
	bool m_expectExpression;
};
