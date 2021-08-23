#include <lexer/rules.hpp>

#include <map>

MatcherResult NoMatch(0, "");

std::vector<Rule> Rules = {
	{ State::Any,                  Comment,             State::Any,                  TokenType::Retry },
	{ State::Any,                  Newline,             State::Any,                  TokenType::Newline },
	{ State::Any,                  Indent,              State::Any,                  TokenType::Indent },
	{ State::Any,                  Whitespace,          State::Any,                  TokenType::Retry },

	{ State::Initial,              ExpressionKeyword,   State::ExpressionKeyword,    TokenType::Keyword },
	{ State::ExpressionKeyword,    Identifier,          State::ExpressionIdentifier, TokenType::Identifier },
	{ State::ExpressionIdentifier, Regex,               State::Initial,              TokenType::Expression },

	{ State::Initial,              PatternKeyword,      State::PatternKeyword,       TokenType::Keyword },
	{ State::PatternKeyword,       Identifier,          State::PatternIdentifier,    TokenType::Identifier },
	{ State::PatternIdentifier,    Alternator,          State::PatternIdentifier,    TokenType::Alternator },
	{ State::PatternIdentifier,    End,                 State::Initial,              TokenType::End },
	{ State::PatternIdentifier,    Identifier,          State::PatternIdentifier,    TokenType::Identifier },

	{ State::Initial,              RuleKeyword,          State::RuleKeyword,         TokenType::Keyword },
	{ State::RuleKeyword,          Identifier,           State::RuleIdentifier,      TokenType::Identifier },
	{ State::RuleIdentifier,       ProduceNothingAction, State::RuleIdentifier,      TokenType::Action},
	{ State::RuleIdentifier,       ProduceAction,        State::RuleProduce,         TokenType::Action},
	{ State::RuleProduce,          Identifier,           State::RuleIdentifier,      TokenType::Identifier },
	{ State::RuleIdentifier,       RewindAction,         State::RuleIdentifier,      TokenType::Action},
	{ State::RuleIdentifier,       TransitionAction,     State::RuleTransition,      TokenType::Action},
	{ State::RuleTransition,       Identifier,           State::RuleIdentifier,      TokenType::Identifier },
	{ State::RuleIdentifier,       LineAction,           State::RuleIdentifier,      TokenType::Action },
	{ State::RuleIdentifier,       MultilineStart,       State::RuleLine,            TokenType::Retry },
	{ State::RuleLine,             MultilineEnd,         State::RuleIdentifier,      TokenType::Action },
	{ State::RuleIdentifier,       End,                  State::Initial,             TokenType::End },

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

MatcherResult End(std::string_view data)
{
	if (data[0] != ';')
	{
		return NoMatch;
	}

	return { 1, ";" };
}

MatcherResult ExpressionKeyword(std::string_view data)
{
	if (!StartsWith(data, "expression"))
	{
		return NoMatch;
	}

	return { sizeof("expression") - 1, "expression" };
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

MatcherResult PatternKeyword(std::string_view data)
{
	if (!StartsWith(data, "pattern"))
	{
		return NoMatch;
	}

	return { sizeof("pattern") - 1, "pattern" };
}

MatcherResult Alternator(std::string_view data)
{
	if (data[0] != '|')
	{
		return NoMatch;
	}

	return { 1, "|" };
}

MatcherResult RuleKeyword(std::string_view data)
{
	if (!StartsWith(data, "rule"))
	{
		return NoMatch;
	}

	return { sizeof("rule") - 1, "rule" };
}

MatcherResult ProduceNothingAction(std::string_view data)
{
	if (!StartsWith(data, "produce-nothing"))
	{
		return NoMatch;
	}

	return { sizeof("produce-nothing") - 1, "produce-nothing" };
}

MatcherResult ProduceAction(std::string_view data)
{
	if (!StartsWith(data, "produce"))
	{
		return NoMatch;
	}

	return { sizeof("produce") - 1, "produce" };
}

MatcherResult RewindAction(std::string_view data)
{
	if (!StartsWith(data, "rewind"))
	{
		return NoMatch;
	}

	return { sizeof("rewind") - 1, "rewind" };
}

MatcherResult TransitionAction(std::string_view data)
{
	if (!StartsWith(data, "transition"))
	{
		return NoMatch;
	}

	return { sizeof("transition") - 1, "transition" };
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

MatcherResult MultilineStart(std::string_view data)
{
	if (!StartsWith(data, "line"))
	{
		return NoMatch;
	}

	return { sizeof("line") - 1, "line" };
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
