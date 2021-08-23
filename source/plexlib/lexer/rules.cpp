#include <lexer/rules.hpp>

#include <map>

MatcherResult NoMatch(0, "");

std::vector<Rule> Rules = {
	{ State::Any,                  Comment,             State::Any,                  TokenType::Retry },
	{ State::Any,                  Newline,             State::Any,                  TokenType::Newline },
	{ State::Any,                  Indent,              State::Any,                  TokenType::Indent },
	{ State::Any,                  Whitespace,          State::Any,                  TokenType::Retry },

	{ State::Initial,              Literal("expression"),   State::ExpressionKeyword,    TokenType::Keyword },
	{ State::ExpressionKeyword,    Identifier,          State::ExpressionIdentifier, TokenType::Identifier },
	{ State::ExpressionIdentifier, Regex,               State::Initial,              TokenType::Expression },

	{ State::Initial,              Literal("pattern"),      State::PatternKeyword,       TokenType::Keyword },
	{ State::PatternKeyword,       Identifier,          State::PatternIdentifier,    TokenType::Identifier },
	{ State::PatternIdentifier,    Literal("|"),          State::PatternIdentifier,    TokenType::Alternator },
	{ State::PatternIdentifier,    Literal(";"),        State::Initial,              TokenType::End },
	{ State::PatternIdentifier,    Identifier,          State::PatternIdentifier,    TokenType::Identifier },

	{ State::Initial,              Literal("rule"),          State::RuleKeyword,         TokenType::Keyword },
	{ State::RuleKeyword,          Identifier,           State::RuleIdentifier,      TokenType::Identifier },
	{ State::RuleIdentifier,       Literal("produce-nothing"), State::RuleIdentifier,      TokenType::Action},
	{ State::RuleIdentifier,       Literal("produce"),        State::RuleProduce,         TokenType::Action},
	{ State::RuleProduce,          Identifier,           State::RuleIdentifier,      TokenType::Identifier },
	{ State::RuleIdentifier,       Literal("rewind"),         State::RuleIdentifier,      TokenType::Action},
	{ State::RuleIdentifier,       Literal("transition"),     State::RuleTransition,      TokenType::Action},
	{ State::RuleTransition,       Identifier,           State::RuleIdentifier,      TokenType::Identifier },
	{ State::RuleIdentifier,       LineAction,           State::RuleIdentifier,      TokenType::Action },
	{ State::RuleIdentifier,       Literal("line"),       State::RuleLine,            TokenType::Retry },
	{ State::RuleLine,             MultilineEnd,         State::RuleIdentifier,      TokenType::Action },
	{ State::RuleIdentifier,       Literal(";"),         State::Initial,             TokenType::End },

	{ State::Any,                  Error,                State::Any,                 TokenType::Unknown },
};

bool StartsWith(const std::string_view& toSearch, const std::string find);

State operator|(State left, State right)
{
	auto lhs = static_cast<std::underlying_type<State>::type>(left);
	auto rhs = static_cast<std::underlying_type<State>::type>(right);
	return static_cast<State>(lhs | rhs);
}

State operator&(State left, State right)
{
	auto lhs = static_cast<std::underlying_type<State>::type>(left);
	auto rhs = static_cast<std::underlying_type<State>::type>(right);
	return static_cast<State>(lhs & rhs);
}

State operator~(State s)
{
	auto st = static_cast<std::underlying_type<State>::type>(s);
	return static_cast<State>(~st);
}

MatcherResult Comment(std::string_view data)
{
	if (data[0] != '#')
	{
		return NoMatch;
	}

	return { data.find_first_of('\n'), "" };
}

MatcherResult Newline(std::string_view data)
{
	if (data[0] != '\n')
	{
		return NoMatch;
	}

	return { 1, "\\n" };
}

MatcherResult Indent(std::string_view data)
{
	if (data[0] != '\t' && !StartsWith(data, "    "))
	{
		return NoMatch;
	}

	size_t size = data[0] == '\t' ? 1 : 4;
	return { size, "\\t" };
}

MatcherResult Whitespace(std::string_view data)
{
	if (data[0] != ' ' && data[0] != '\t')
	{
		return NoMatch;
	}

	return { data.find_first_not_of(" \t"), "" };
}

MatcherResult Identifier(std::string_view data)
{
	size_t size = data.find_first_of(" \t\r\n");
	return { size, std::string(data.substr(0, size)) };
}

MatcherResult Regex(std::string_view data)
{
	size_t size = data.find_first_of("\r\n");
	std::string text(data.substr(0, size));

	if (text.size() > 1 && text[0] == '\\' && text[1] == ' ')
	{
		text.erase(0, 1);
	}

	return { size, text };
}

MatcherResult LineAction(std::string_view data)
{
	if (StartsWith(data, "++line") || StartsWith(data, "line++"))
	{
		return { sizeof("line++") - 1, "+1" };
	}

	if (StartsWith(data, "--line") || StartsWith(data, "line--"))
	{
		return { sizeof("line--") - 1, "-1" };
	}

	return NoMatch;
}

MatcherResult MultilineEnd(std::string_view data)
{
	size_t size = data.find_first_of(" \t\r\n");
	std::string text(data.substr(0, size));
	return { size, text };
}

MatcherResult Error(std::string_view data)
{
	return { 1, std::string(1, data[0]) };
}

Matcher Literal(std::string value)
{
	return [value](std::string_view data)
	{
		if (!StartsWith(data, value))
		{
			return NoMatch;
		}

		return MatcherResult(value.size(), value);
	};
}
