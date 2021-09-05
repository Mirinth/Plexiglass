#include "lexer.hpp"

#include <lexer/rules.hpp>

constexpr char* whitespace = " \t\r\n";

std::string_view StripComment(std::string_view line);
bool IsBlank(std::string_view line);

/// <summary>
/// Construct a lexer.
/// </summary>
/// <param name="data">The data to lex.</param>
Lexer::Lexer(std::string_view data)
	: m_data(data)
	, m_lineNumber(0)
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

	m_lineNumber++;

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
void Lexer::LexLine(std::string_view line)
{
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
	while (m_buffer.empty() && !m_data.empty())
	{
		std::string_view line = GetLine();
		line = StripComment(line);
		if (!IsBlank(line))
		{
			LexLine(line);
		}
	}

	if (m_buffer.empty())
	{
		m_buffer.push(Token(TokenType::Eof));
	}
}

/// <summary>
/// Remove the comment from the end of a line, if any.
/// </summary>
/// <param name="line">Line to remove the comment from.</param>
/// <returns>The line, with any comments removed.</returns>
std::string_view StripComment(std::string_view line)
{
	size_t marker = line.find_first_of("#");
	if (marker == std::string_view::npos)
	{
		return line;
	}

	return line.substr(0, marker);
}

/// <summary>
/// Check whether a line is empty or consists only of whitespace.
/// </summary>
/// <param name="line">The line to check.</param>
/// <returns>Whether the line is blank.</returns>
bool IsBlank(std::string_view line)
{
	return (
		line.empty()
		|| line[0] == '\n'
		|| line.find_first_not_of(whitespace) == std::string_view::npos
		);
}
