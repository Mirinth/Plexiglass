#include "lexer.hpp"

#include <fstream>
#include <tuple>

namespace
{
	Token Lex(std::string_view& data);
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
	, m_current(Lex(m_data))
	, m_next(Lex(m_data))
{
}

void Lexer::Shift()
{
	m_current = m_next;
	m_next = Lex(m_data);
}

const Token& Lexer::Current() const
{
	return m_current;
}

const Token& Lexer::Next() const
{
	return m_next;
}

namespace
{
	Token Lex(std::string_view& data)
	{
		return Token(99, TokenType::Eof, "This is a token");
	}
}