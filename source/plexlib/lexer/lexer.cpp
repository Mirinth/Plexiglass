#include "lexer.hpp"

#include <fstream>
#include <vector>

Lexer::Lexer(std::string_view data)
	: m_data(data)
	, m_line(1)
	, m_state(State::Initial)
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

	for (auto& [current, matcher, next, type] : Rules)
	{
		if ((m_state & current) == State::Invalid)
		{
			continue;
		}

		auto[size, text] = matcher(m_data);

		if (size == 0)
		{
			continue;
		}

		m_data.remove_prefix(size);
		if (next != State::Any)
		{
			m_state = next;
		}
		if (type == TokenType::Newline)
		{
			m_line++;
		}
		return Token(m_line, type, text);
	}

	throw std::exception("Unreachable code reached.");
}
