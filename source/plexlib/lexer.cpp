#include "lexer.hpp"

#include <fstream>

constexpr int invalid_state = -1;
constexpr int initial_state = 0;

constexpr int expression_keyword_state = 1;
constexpr int expression_identifier_state = 2;

constexpr int pattern_keyword_state = 11;
constexpr int pattern_identifier_state = 12;

constexpr int rule_keyword_state = 21;
constexpr int rule_identifier_state = 22;
constexpr int rule_produce_state = 23;
constexpr int rule_transition_state = 24;
constexpr int rule_line_state = 25;


bool StartsWith(const std::string_view& toSearch, const std::string find);
std::string TakeUntil(std::string_view& data, const std::string find);

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
	std::string name;
	switch (token.type)
	{
	case TokenType::Action:
		name = "action";
		break;
	case TokenType::Alternator:
		name = "alternator";
		break;
	case TokenType::End:
		name = "end";
		break;
	case TokenType::Eof:
		name = "eof";
		break;
	case TokenType::Expression:
		name = "expression";
		break;
	case TokenType::Identifier:
		name = "identifier";
		break;
	case TokenType::Keyword:
		name = "keyword";
		break;
	case TokenType::Newline:
		name = "newline";
		break;
	case TokenType::Unknown:
		name = "unknown";
		break;
	default:
		throw std::exception("Unrecognized token type in operator<<(Token)");
	}

	os << name << " " << token.text;
	return os;
}

Lexer::Lexer(std::string_view data)
	: m_data(data)
	, m_line(1)
	, m_state(initial_state)
{
	Lex();
	Shift();
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

#define KEYWORD(name, next) \
	if (m_state == initial_state && StartsWith(m_data, name)) \
    { \
	    m_next = Token(m_line, TokenType::Keyword, name); \
		m_data.remove_prefix(sizeof(name) - 1); \
		m_state = next; \
		return; \
	}

#define IDENTIFIER(current, next) \
	if (m_state == current) \
	{ \
		std::string identifier = TakeUntil(m_data, " \t\r\n"); \
		m_next = Token(m_line, TokenType::Identifier, identifier); \
		m_state = next; \
		return; \
	}

#define LINE_ACTION(search_name, token_name, next) \
	if (m_state == rule_identifier_state && StartsWith(m_data, search_name)) \
	{ \
		m_next = Token(m_line, TokenType::Action, token_name); \
		m_data.remove_prefix(sizeof(search_name) - 1); \
		m_state = next; \
		return; \
	}

#define ACTION(name, next) \
	LINE_ACTION(name, name, next)

void Lexer::Lex()
{
	// Any state
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

	if (m_data[0] == ' ' || m_data[0] == '\t')
	{
		while (m_data[0] == ' ' || m_data[0] == '\t')
		{
			m_data.remove_prefix(1);
		}
		Lex();
		return;
	}

	// Comments
	if (m_state == initial_state && m_data[0] == '#')
	{
		while (m_data[0] != '\n')
		{
			m_data.remove_prefix(1);
		}
		Lex();
		return;
	}

	// Expressions
	KEYWORD("expression", expression_keyword_state);

	IDENTIFIER(expression_keyword_state, expression_identifier_state);

	if (m_state == expression_identifier_state)
	{
		std::string expression = TakeUntil(m_data, "\r\n");
		if (expression.size() > 1 && expression[0] == '\\')
		{
			if (expression[1] == ' ' || expression[1] == '\t')
			{
				expression.erase(0, 1);
			}
		}
		m_next = Token(m_line, TokenType::Expression, expression);
		m_state = initial_state;
		return;
	}

	// Patterns
	KEYWORD("pattern", pattern_keyword_state);

	IDENTIFIER(pattern_keyword_state, pattern_identifier_state);

	if (m_state == pattern_identifier_state && m_data[0] == '|')
	{
		m_next = Token(m_line, TokenType::Alternator, "|");
		m_data.remove_prefix(1);
		return;
	}

	if (m_state == pattern_identifier_state && m_data[0] == ';')
	{
		m_next = Token(m_line, TokenType::End, ";");
		m_data.remove_prefix(1);
		m_state = initial_state;
		return;
	}

	IDENTIFIER(pattern_identifier_state, pattern_identifier_state);

	// Rules
	KEYWORD("rule", rule_keyword_state);

	IDENTIFIER(rule_keyword_state, rule_identifier_state);

	if (m_state == rule_identifier_state && m_data[0] == ';')
	{
		m_next = Token(m_line, TokenType::End, ";");
		m_data.remove_prefix(1);
		m_state = initial_state;
		return;
	}

	ACTION("produce-nothing", rule_identifier_state);

	ACTION("produce", rule_produce_state);

	IDENTIFIER(rule_produce_state, rule_identifier_state);

	ACTION("rewind", rule_identifier_state);

	ACTION("transition", rule_transition_state);

	IDENTIFIER(rule_transition_state, rule_identifier_state);

	LINE_ACTION("line++", "+1", rule_identifier_state);

	LINE_ACTION("++line", "+1", rule_identifier_state);

	LINE_ACTION("line--", "-1", rule_identifier_state);

	LINE_ACTION("--line", "-1", rule_identifier_state);

	if (m_state == rule_identifier_state && StartsWith(m_data, "line"))
	{
		m_data.remove_prefix(sizeof("line"));
		m_state = rule_line_state;
		return;
	}

	if (m_state == rule_line_state)
	{
		std::string identifier = TakeUntil(m_data, " \t\r\n");
		m_next = Token(m_line, TokenType::Action, identifier);
		m_state = rule_identifier_state;
		return;
	}

	/*
	* Things to handle:
indentifier some-name
indent
expression [a-bA-Y09]lyoun(kkol)?
identifier another-name
expression [a-z0-9]{3}
identifier third-name
expression  expression with space
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
