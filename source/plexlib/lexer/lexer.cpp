#include "lexer.hpp"

#include <lexer/rules.hpp>

/// <summary>
/// Construct a lexer.
/// </summary>
/// <param name="data">The data to lex.</param>
Lexer::Lexer(std::string_view data)
	: m_data(data)
	, m_lineNumber(1)
{
	FillBuffer();
}

/// <summary>
/// Advance the lexer to the next token.
/// </summary>
void Lexer::Shift()
{
	m_buffer.pop();
	FillBuffer();
}

/// <summary>
/// Get the current token.
/// </summary>
/// <returns>The current token.</returns>
const Token& Lexer::Peek() const
{
	return m_buffer.front();
}

Token Lexer::LexHelper(std::string_view& line)
{
	if (line.empty())
	{
		return Token(m_lineNumber, TokenType::Eof, "EOF");
	}

	size_t longestSize = 0;
	TokenType longestToken = TokenType::Unknown;

	for (auto& [matcher, type] : Rules)
	{
		size_t size = matcher(line);
		if (size > longestSize)
		{
			longestSize = size;
			longestToken = type;
		}
	}

	std::string text(line.substr(0, longestSize));
	line.remove_prefix(longestSize);
	
	if (longestToken == TokenType::Newline)
	{
		m_lineNumber++;
		return Token(m_lineNumber, TokenType::Retry, "");
	}

	return Token(m_lineNumber, longestToken, text);
}

/// <summary>
/// Get the first line in m_data. After returning, m_data will be modified
/// to remove the returned line from its content.
/// </summary>
/// <returns>
/// The first line in m_data. All lines but the last will have a trailing
/// newline. May be empty if m_data is empty.
/// </returns>
std::string_view Lexer::GetLine()
{
	if (m_data.empty())
	{
		return std::string_view();
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

/// <summary>
/// Break an entire line into tokens, pushing them all into m_buffer.
/// m_buffer may still be empty after a call to LexLine if the line
/// contained no tokens.
/// </summary>
void Lexer::LexLine()
{
	if (m_data.empty())
	{
		m_buffer.push(Token(TokenType::Eof));
		return;
	}

	std::string_view line = GetLine();
	while (!line.empty())
	{
		Token tok = LexHelper(line);
		if (tok.type != TokenType::Retry)
		{
			m_buffer.push(tok);
		}
	}
}

/// <summary>
/// Ensure there is at least one token in the buffer.
/// </summary>
void Lexer::FillBuffer()
{
	while (m_buffer.empty())
	{
		LexLine();
	}
}
