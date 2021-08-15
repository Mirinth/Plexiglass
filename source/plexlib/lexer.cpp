#include "lexer.hpp"

#include <fstream>

Token::Token()
	: line(0), type(TokenType::Eof), text("")
{
}

Token::Token(size_t line, TokenType type, std::string text)
	:line(line), type(type), text(text)
{
}

std::ostream& operator<<(std::ostream& os, const Token token)
{
	os << token.text;
	return os;
}

Lexer::Lexer(std::string_view data)
	: m_data(data)
	, m_line(1)
{
	Lex();
	Shift();
	Lex();
}

void Lexer::Shift()
{
	m_current = m_next;
	Lex();
}

const Token& Lexer::Current() const
{
	return m_current;
}

const Token& Lexer::Next() const
{
	return m_next;
}

void Lexer::Lex()
{
	if (!m_data.empty())
	{
		m_next = Token(99, TokenType::Identifier, "This is a token");
		size_t amount = std::min(size_t(50), m_data.size());
		m_data.remove_prefix(amount);
	}
	else
	{
		m_next = Token(95, TokenType::Eof, "This is the end of file");
	}
}
