#include "lexer.hpp"

#include <fstream>


enum class LexerState
{
	Invalid = 0,
	Initial = 1,

	ExpressionKeyword = 2,
	ExpressionIdentifier = 4,

	PatternKeyword = 8,
	PatternIdentifier = 16,

	RuleKeyword = 32,
	RuleIdentifier = 64,
	RuleProduce = 128,
	RuleTransition = 256,
	RuleLine = 512,
};

bool StartsWith(const std::string_view& toSearch, const std::string find);
std::string TakeUntil(std::string_view& data, const std::string find);



Lexer::Lexer(std::string_view data)
	: m_data(data)
	, m_line(1)
	, m_state(LexerState::Initial)
	, m_startOfLine(true)
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
	if (m_state == LexerState::Initial && StartsWith(m_data, name)) \
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
	if (m_state == LexerState::RuleIdentifier && StartsWith(m_data, search_name)) \
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

	if (m_data[0] == '\n')
	{
		Token tok(m_line, TokenType::Newline, "\\n");
		m_line++;
		m_data.remove_prefix(1);
		m_startOfLine = true;
		return tok;
	}

	if (m_startOfLine && m_data[0] == '\t' || StartsWith(m_data, "    "))
	{
		Token tok(m_line, TokenType::Indent, "\\t");
		size_t amount = m_data[0] == '\t' ? 1 : sizeof("    ") - 1;
		m_data.remove_prefix(amount);
		return tok;
	}

	m_startOfLine = false;

	if (m_data[0] == ' ' || m_data[0] == '\t')
	{
		while (m_data[0] == ' ' || m_data[0] == '\t')
		{
			m_data.remove_prefix(1);
		}
		return Token(0, TokenType::Retry, "");
	}

	// Comments
	if (m_data[0] == '#')
	{
		while (m_data[0] != '\n')
		{
			m_data.remove_prefix(1);
		}
		return Token(0, TokenType::Retry, "");
	}

	// Expressions
	KEYWORD("expression", LexerState::ExpressionKeyword);

	IDENTIFIER(LexerState::ExpressionKeyword, LexerState::ExpressionIdentifier);

	if (m_state == LexerState::ExpressionIdentifier)
	{
		std::string expression = TakeUntil(m_data, "\r\n");
		if (expression.size() > 1 && expression[0] == '\\')
		{
			if (expression[1] == ' ' || expression[1] == '\t')
			{
				expression.erase(0, 1);
			}
		}
		Token tok(m_line, TokenType::Expression, expression);
		m_state = LexerState::Initial;
		return tok;
	}

	// Patterns
	KEYWORD("pattern", LexerState::PatternKeyword);

	IDENTIFIER(LexerState::PatternKeyword, LexerState::PatternIdentifier);

	if (m_state == LexerState::PatternIdentifier && m_data[0] == '|')
	{
		Token tok(m_line, TokenType::Alternator, "|");
		m_data.remove_prefix(1);
		return tok;
	}

	if (m_state == LexerState::PatternIdentifier && m_data[0] == ';')
	{
		Token tok(m_line, TokenType::End, ";");
		m_data.remove_prefix(1);
		m_state = LexerState::Initial;
		return tok;
	}

	IDENTIFIER(LexerState::PatternIdentifier, LexerState::PatternIdentifier);

	// Rules
	KEYWORD("rule", LexerState::RuleKeyword);

	IDENTIFIER(LexerState::RuleKeyword, LexerState::RuleIdentifier);

	if (m_state == LexerState::RuleIdentifier && m_data[0] == ';')
	{
		Token tok(m_line, TokenType::End, ";");
		m_data.remove_prefix(1);
		m_state = LexerState::Initial;
		return tok;
	}

	ACTION("produce-nothing", LexerState::RuleIdentifier);

	ACTION("produce", LexerState::RuleProduce);

	IDENTIFIER(LexerState::RuleProduce, LexerState::RuleIdentifier);

	ACTION("rewind", LexerState::RuleIdentifier);

	ACTION("transition", LexerState::RuleTransition);

	IDENTIFIER(LexerState::RuleTransition, LexerState::RuleIdentifier);

	LINE_ACTION("line++", "+1", LexerState::RuleIdentifier);

	LINE_ACTION("++line", "+1", LexerState::RuleIdentifier);

	LINE_ACTION("line--", "-1", LexerState::RuleIdentifier);

	LINE_ACTION("--line", "-1", LexerState::RuleIdentifier);

	if (m_state == LexerState::RuleIdentifier && StartsWith(m_data, "line"))
	{
		m_data.remove_prefix(sizeof("line"));
		m_state = LexerState::RuleLine;
		return Token(0, TokenType::Retry, "");
	}

	if (m_state == LexerState::RuleLine)
	{
		std::string identifier = TakeUntil(m_data, " \t\r\n");
		Token tok(m_line, TokenType::Action, identifier);
		m_state = LexerState::RuleIdentifier;
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
