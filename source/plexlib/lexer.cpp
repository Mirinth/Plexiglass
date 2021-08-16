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
	if (m_state == initial_state && StartsWith(m_data, "expression"))
	{
		m_next = Token(m_line, TokenType::Keyword, "expression");
		m_data.remove_prefix(sizeof("expression"));
		m_state = expression_keyword_state;
		return;
	}

	if (m_state == expression_keyword_state)
	{
		std::string identifier = TakeUntil(m_data, " \t\r\n");
		m_next = Token(m_line, TokenType::Identifier, identifier);
		m_state = expression_identifier_state;
		return;
	}

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
	if (m_state == initial_state && StartsWith(m_data, "pattern"))
	{
		m_next = Token(m_line, TokenType::Keyword, "pattern");
		m_data.remove_prefix(sizeof("pattern"));
		m_state = pattern_keyword_state;
		return;
	}

	if (m_state == pattern_keyword_state)
	{
		std::string identifier = TakeUntil(m_data, " \t\r\n");
		m_next = Token(m_line, TokenType::Identifier, identifier);
		m_state = pattern_identifier_state;
		return;
	}

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

	if (m_state == pattern_identifier_state)
	{
		std::string identifier = TakeUntil(m_data, " \t\r\n");
		m_next = Token(m_line, TokenType::Identifier, identifier);
		return;
	}

	// Rules
	if (m_state == initial_state && StartsWith(m_data, "rule"))
	{
		m_next = Token(m_line, TokenType::Keyword, "rule");
		m_data.remove_prefix(sizeof("rule"));
		m_state = rule_keyword_state;
		return;
	}

	if (m_state == rule_keyword_state)
	{
		std::string identifier = TakeUntil(m_data, " \t\r\n");
		m_next = Token(m_line, TokenType::Identifier, identifier);
		m_state = rule_identifier_state;
		return;
	}

	if (m_state == rule_identifier_state && m_data[0] == ';')
	{
		m_next = Token(m_line, TokenType::End, ";");
		m_data.remove_prefix(1);
		m_state = initial_state;
		return;
	}

	if (m_state == rule_identifier_state && StartsWith(m_data, "produce-nothing"))
	{
		m_next = Token(m_line, TokenType::Action, "produce-nothing");
		m_data.remove_prefix(sizeof("produce-nothing"));
		return;
	}

	if (m_state == rule_identifier_state && StartsWith(m_data, "produce"))
	{
		m_next = Token(m_line, TokenType::Action, "produce");
		m_data.remove_prefix(sizeof("produce"));
		m_state = rule_produce_state;
		return;
	}

	if (m_state == rule_produce_state)
	{
		std::string identifier = TakeUntil(m_data, " \t\r\n");
		m_next = Token(m_line, TokenType::Identifier, identifier);
		m_state = rule_identifier_state;
		return;
	}

	if (m_state == rule_identifier_state && StartsWith(m_data, "rewind"))
	{
		m_next = Token(m_line, TokenType::Action, "rewind");
		m_data.remove_prefix(sizeof("rewind"));
		return;
	}

	if (m_state == rule_identifier_state && StartsWith(m_data, "transition"))
	{
		m_next = Token(m_line, TokenType::Action, "transition");
		m_data.remove_prefix(sizeof("transition"));
		m_state = rule_transition_state;
		return;
	}

	if (m_state == rule_transition_state)
	{
		std::string identifier = TakeUntil(m_data, " \t\r\n");
		m_next = Token(m_line, TokenType::Identifier, identifier);
		m_state = rule_identifier_state;
		return;
	}

	if (m_state == rule_identifier_state && StartsWith(m_data, "++line") || StartsWith(m_data, "line++"))
	{
		m_next = Token(m_line, TokenType::Action, "+1");
		m_data.remove_prefix(sizeof("line++"));
		return;
	}

	if (m_state == rule_identifier_state && StartsWith(m_data, "--line") || StartsWith(m_data, "line--"))
	{
		m_next = Token(m_line, TokenType::Action, "-1");
		m_data.remove_prefix(sizeof("line--"));
		return;
	}

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
