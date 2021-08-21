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

#define KEYWORD(name, next) \
	if (m_state == State::Initial && StartsWith(m_data, name)) \
    { \
	    Token tok(m_line, TokenType::Keyword, name); \
		m_data.remove_prefix(sizeof(name) - 1); \
		m_state = next; \
		return tok; \
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

#define ACTION(name, next) \
	LINE_ACTION(name, name, next)

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

	for (auto& rule : Rules)
	{
		State next = State::Invalid;
		TokenType type = TokenType::Unknown;
		std::string text;
		size_t length = rule(m_data, m_state, next, type, text);

		if (length)
		{
			Token tok(m_line, type, text);
			m_data.remove_prefix(length);

			if (type == TokenType::Newline)
			{
				m_line++;
			}

			m_state = next;

			return tok;
		}
	}

	// Patterns
	KEYWORD("pattern", State::PatternKeyword);

	IDENTIFIER(State::PatternKeyword, State::PatternIdentifier);

	if (m_state == State::PatternIdentifier && m_data[0] == '|')
	{
		Token tok(m_line, TokenType::Alternator, "|");
		m_data.remove_prefix(1);
		return tok;
	}

	if (m_state == State::PatternIdentifier && m_data[0] == ';')
	{
		Token tok(m_line, TokenType::End, ";");
		m_data.remove_prefix(1);
		m_state = State::Initial;
		return tok;
	}

	IDENTIFIER(State::PatternIdentifier, State::PatternIdentifier);

	// Rules
	KEYWORD("rule", State::RuleKeyword);

	IDENTIFIER(State::RuleKeyword, State::RuleIdentifier);

	if (m_state == State::RuleIdentifier && m_data[0] == ';')
	{
		Token tok(m_line, TokenType::End, ";");
		m_data.remove_prefix(1);
		m_state = State::Initial;
		return tok;
	}

	ACTION("produce-nothing", State::RuleIdentifier);

	ACTION("produce", State::RuleProduce);

	IDENTIFIER(State::RuleProduce, State::RuleIdentifier);

	ACTION("rewind", State::RuleIdentifier);

	ACTION("transition", State::RuleTransition);

	IDENTIFIER(State::RuleTransition, State::RuleIdentifier);

	LINE_ACTION("line++", "+1", State::RuleIdentifier);

	LINE_ACTION("++line", "+1", State::RuleIdentifier);

	LINE_ACTION("line--", "-1", State::RuleIdentifier);

	LINE_ACTION("--line", "-1", State::RuleIdentifier);

	if (m_state == State::RuleIdentifier && StartsWith(m_data, "line"))
	{
		m_data.remove_prefix(sizeof("line"));
		m_state = State::RuleLine;
		return Token(0, TokenType::Retry, "");
	}

	if (m_state == State::RuleLine)
	{
		std::string identifier = TakeUntil(m_data, " \t\r\n");
		Token tok(m_line, TokenType::Action, identifier);
		m_state = State::RuleIdentifier;
		return tok;
	}

	Token tok(m_line, TokenType::Unknown, std::string(1, m_data[0]));
	m_data.remove_prefix(1);
	return tok;
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
