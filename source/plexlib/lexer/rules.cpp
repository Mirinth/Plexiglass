#include <lexer/rules.hpp>

#include <map>

MatcherResult NoMatch(0, "");

std::vector<Rule> Rules = {
	{ State::Any,                  Comment,                    State::Any,                  TokenType::Retry },
	{ State::Any,                  Literal("\n", "\\n"),       State::Any,                  TokenType::Newline },
	{ State::Any,                  Whitespace,                 State::Any,                  TokenType::Retry },

	{ State::Initial,              Literal("expression"),      State::ExpressionKeyword,    TokenType::KwdExpression },
	{ State::ExpressionKeyword,    Identifier,                 State::ExpressionIdentifier, TokenType::Identifier },
	{ State::ExpressionIdentifier, Regex,                      State::Initial,              TokenType::Regex },

	{ State::Initial,              Literal("pattern"),         State::PatternKeyword,       TokenType::KwdPattern },
	{ State::PatternKeyword,       Identifier,                 State::PatternIdentifier,    TokenType::Identifier },
	{ State::PatternIdentifier,    Literal("|"),               State::PatternIdentifier,    TokenType::Alternator },
	{ State::PatternIdentifier,    Literal(";"),               State::Initial,              TokenType::End },
	{ State::PatternIdentifier,    Identifier,                 State::PatternIdentifier,    TokenType::Identifier },

	{ State::Initial,              Literal("rule"),            State::RuleKeyword,         TokenType::KwdRule },
	{ State::RuleKeyword,          Identifier,                 State::RuleIdentifier,      TokenType::Identifier },
	{ State::RuleIdentifier,       Literal("produce-nothing"), State::RuleIdentifier,      TokenType::ActProduceNil },
	{ State::RuleIdentifier,       Literal("produce"),         State::RuleProduce,         TokenType::ActProduce },
	{ State::RuleProduce,          Identifier,                 State::RuleIdentifier,      TokenType::Identifier },
	{ State::RuleIdentifier,       Literal("rewind"),          State::RuleIdentifier,      TokenType::ActRewind },
	{ State::RuleIdentifier,       Literal("transition"),      State::RuleTransition,      TokenType::ActTransition },
	{ State::RuleTransition,       Identifier,                 State::RuleIdentifier,      TokenType::Identifier },
	
	{ State::RuleIdentifier,       Literal("++line", "+1"),    State::RuleIdentifier,      TokenType::ActInc },
	{ State::RuleIdentifier,       Literal("line++", "+1"),    State::RuleIdentifier,      TokenType::ActInc },
	{ State::RuleIdentifier,       Literal("--line", "-1"),    State::RuleIdentifier,      TokenType::ActDec },
	{ State::RuleIdentifier,       Literal("line--", "-1"),    State::RuleIdentifier,      TokenType::ActDec },

	{ State::RuleIdentifier,       Literal(";"),		       State::Initial,             TokenType::End },

	{ State::Any,                  Error,                      State::Any,                 TokenType::Unknown },
};

State operator&(State left, State right)
{
	auto lhs = static_cast<std::underlying_type<State>::type>(left);
	auto rhs = static_cast<std::underlying_type<State>::type>(right);
	return static_cast<State>(lhs & rhs);
}

MatcherResult Comment(std::string_view data)
{
	if (data[0] != '#')
	{
		return NoMatch;
	}

	return { data.find_first_of('\n'), "" };
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

Matcher Literal(std::string match, std::string produce)
{
	return [match, produce](std::string_view data)
	{
		if (!StartsWith(data, match))
		{
			return NoMatch;
		}

		return MatcherResult(match.size(), produce);
	};
}
