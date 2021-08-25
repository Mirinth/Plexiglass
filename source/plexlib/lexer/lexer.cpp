#include "lexer.hpp"

#include <fstream>
#include <vector>

Lexer::Lexer(std::string_view data)
	: m_data(data)
	, m_line(1)
{
	m_current = Lex();
	m_next = Lex();
}

void Lexer::Shift()
{
	m_current = m_next;
	m_next = Lex();
}

const Token& Lexer::Current() const
{
	return m_current;
}

const Token& Lexer::Next() const
{
	return m_next;
}

Token Lexer::Lex()
{
	Token tok = LexHelper();
	while (tok.type == TokenType::Retry)
	{
		tok = LexHelper();
	}
	return tok;
}

Token Lexer::LexHelper()
{
	// Any state
	if (m_data.empty())
	{
		return Token(m_line, TokenType::Eof, "EOF");
	}

	size_t longestSize = 0;
	TokenType longestToken = TokenType::Unknown;

	for (auto& [matcher, type] : Rules)
	{
		size_t size = matcher(m_data);
		if (size > longestSize)
		{
			longestSize = size;
			longestToken = type;
		}
	}

	std::string text(m_data.substr(0, longestSize));
	m_data.remove_prefix(longestSize);
	
	if (longestToken == TokenType::Newline)
	{
		m_line++;
		return Token(m_line, TokenType::Retry, "");
	}

	return Token(m_line, longestToken, text);
}
