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
	if (m_data.empty())
	{
		m_next = Token(m_line, TokenType::Eof, "EOF");
		return;
	}

	if (m_data[0] == '\n')
	{
		m_next = Token(m_line, TokenType::Newline, "\\n");
		m_line++;
		m_data.remove_prefix(1);
		return;
	}

	/*
	* Things to handle:
keyword expression
indentifier some-name
indent
expression [a-bA-Y09]lyoun(kkol)?
keyword expression
identifier another-name
expression [a-z0-9]{3}
keyword expression
identifier third-name
expression  expression with space
keyword pattern
identifier some-name#
identifier comments
identifier aren't
identifier allowed
identifier at
identifier the
identifier end
identifier of
identifier the
identifier line
indent
identifier abcde
indent
operator |
identifier fghij
identifier klmno
identifier pqrs
indent
operator |
identifier wxyz
keyword rule
identifier what-to-match
indent
action produce-nothing
indent
action produce
identifier token-name
indent
action rewind
indent
action transition
identifier another-state
indent
action inc-line
indent
action inc-line
indent
action dec-line
indent
action dec-line
indent
action line
number +2
indent
action line
number -9
	*/

	m_next = Token(m_line, TokenType::Unknown, std::string(1, m_data[0]));
	m_data.remove_prefix(1);
}
