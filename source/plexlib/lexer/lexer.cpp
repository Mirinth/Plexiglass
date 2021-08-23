#include "lexer.hpp"

#include <fstream>
#include <vector>


bool StartsWith(const std::string_view& toSearch, const std::string find);
std::string TakeUntil(std::string_view& data, const std::string find);

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

#define IDENTIFIER(current, next) \
	if (m_state == current) \
	{ \
		std::string identifier = TakeUntil(m_data, " \t\r\n"); \
		Token tok(m_line, TokenType::Identifier, identifier); \
		m_state = next; \
		return tok; \
	}

#define LINE_ACTION(search_name, token_name, next) \
	if (m_state == State::RuleIdentifier && StartsWith(m_data, search_name)) \
	{ \
		Token tok(m_line, TokenType::Action, token_name); \
		m_data.remove_prefix(sizeof(search_name) - 1); \
		m_state = next; \
		return tok; \
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

		auto& [size, text] = matcher(m_data);

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
			m_state = m_state | State::StartOfLine;
		}
		else
		{
			m_state = m_state & (~State::StartOfLine);
		}
		return Token(m_line, type, text);
	}

	throw std::exception("Unreachable code reached.");
}

bool StartsWith(const std::string_view& toSearch, const std::string find)
{
	for (size_t i = 0; i < toSearch.size() && i < find.size(); i++)
	{
		if (toSearch[i] != find[i])
		{
			return false;
		}
	}

	return true;
}

std::string TakeUntil(std::string_view& data, const std::string find)
{
	size_t end = data.find_first_of(find);
	auto viewIdentifier = data.substr(0, end);
	std::string identifier(viewIdentifier.begin(), viewIdentifier.end());
	data.remove_prefix(end);
	return identifier;
}
