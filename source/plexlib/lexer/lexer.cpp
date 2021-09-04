#include "lexer.hpp"

#include <fstream>
#include <vector>

#include <lexer/rules.hpp>

Lexer::Lexer(std::string_view data)
	: m_data(data)
	, m_lineNumber(1)
{
	FillBuffer();
}

void Lexer::Shift()
{
	m_buffer.pop();
	FillBuffer();
}

const Token& Lexer::Peek() const
{
	return m_buffer.front();
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
	m_line = GetLine();
	if (m_line.empty())
	{
		return Token(m_lineNumber, TokenType::Eof, "EOF");
	}

	size_t longestSize = 0;
	TokenType longestToken = TokenType::Unknown;

	for (auto& [matcher, type] : Rules)
	{
		size_t size = matcher(m_line);
		if (size > longestSize)
		{
			longestSize = size;
			longestToken = type;
		}
	}

	std::string text(m_line.substr(0, longestSize));
	m_line.remove_prefix(longestSize);
	
	if (longestToken == TokenType::Newline)
	{
		m_lineNumber++;
		return Token(m_lineNumber, TokenType::Retry, "");
	}

	return Token(m_lineNumber, longestToken, text);
}

std::string_view Lexer::GetLine()
{
	if (!m_line.empty() || m_data.empty())
	{
		return m_line;
	}

	std::string_view line;
	size_t size = m_data.find('\n');
	if (size == std::string_view::npos)
	{
		line = m_data;
		m_data.remove_prefix(m_data.size());
	}
	else
	{
		line = m_data.substr(0, size + 1); // include the newline
		m_data.remove_prefix(size + 1);
	}

	return line;
}

void Lexer::FillBuffer()
{
	m_buffer.push(Lex());
}
