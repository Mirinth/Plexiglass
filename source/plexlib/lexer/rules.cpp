#include <lexer/rules.hpp>

#include <map>

MatcherResult NoMatch(0, "");

// State, Matcher2, State, TokenType
std::vector<Rule> Rules = {
	{ State::Any, Comment, State::Any, TokenType::Retry },
	{ State::Any, Newline, State::Any, TokenType::Newline },
	{ State::Any, Indent, State::Any, TokenType::Indent },
	{ State::Any, Whitespace, State::Any, TokenType::Retry },
	
	{ State::Initial, ExpressionKeyword, State::ExpressionKeyword, TokenType::Keyword },
	{ State::ExpressionKeyword, Identifier, State::ExpressionIdentifier, TokenType::Identifier },
	{ State::ExpressionIdentifier, Regex, State::Initial, TokenType::Expression },

	{ State::Initial, PatternKeyword, State::PatternKeyword, TokenType::Keyword },
	{ State::PatternKeyword, Identifier, State::PatternIdentifier, TokenType::Identifier },
	{ State::PatternIdentifier, Alternator, State::PatternIdentifier, TokenType::Alternator },
	{ State::PatternIdentifier, End, State::Initial, TokenType::End },

	{ State::Initial, RuleKeyword, State::RuleKeyword, TokenType::Keyword },
	{ State::RuleKeyword, Identifier, State::RuleIdentifier, TokenType::Identifier },
	{ State::RuleIdentifier, End, State::Initial, TokenType::End },
};

std::vector<Matcher> Matchers = {
	&Action,
	&LineAction,
	&LineMulti,
	// Identifier needs to come after everything but the error rule since it
	// interferes with everything that follows it.
	&OldIdentifier,
	&Error,
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

size_t OldIdentifier(std::string_view data, State current, State& next, TokenType& type, std::string& text)
{
	static std::map<State, State> nextState = {
		{State::PatternIdentifier, State::PatternIdentifier},
		{State::RuleProduce, State::RuleIdentifier},
		{State::RuleTransition, State::RuleIdentifier},
	};

	if (nextState.count(current) == 0)
	{
		return 0;
	}

	next = nextState[current];
	type = TokenType::Identifier;
	size_t size = data.find_first_of(" \t\r\n");
	text = data.substr(0, size);
	return size;
}

size_t Action(std::string_view data, State current, State& next, TokenType& type, std::string& text)
{
	if ((current & State::RuleIdentifier) == State::Invalid)
	{
		return 0;
	}

	static std::vector<std::tuple<std::string, State>> actions = {
		{"produce-nothing", State::RuleIdentifier},
		{"produce", State::RuleProduce},
		{"rewind", State::RuleIdentifier},
		{"transition", State::RuleTransition},
	};

	for (auto& [name, state] : actions)
	{
		if (StartsWith(data, name))
		{
			next = state;
			type = TokenType::Action;
			text = name;
			return text.size();
		}
	}

	return 0;
}

size_t LineAction(std::string_view data, State current, State& next, TokenType& type, std::string& text)
{
	if ((current & State::RuleIdentifier) == State::Invalid)
	{
		return 0;
	}
	
	if (StartsWith(data, "++line") || StartsWith(data, "line++"))
	{
		next = State::RuleIdentifier;
		type = TokenType::Action;
		text = "+1";
		return sizeof("line++") - 1;
	}

	if (StartsWith(data, "--line") || StartsWith(data, "line--"))
	{
		next = State::RuleIdentifier;
		type = TokenType::Action;
		text = "-1";
		return sizeof("line--") - 1;
	}

	return 0;
}

size_t LineMulti(std::string_view data, State current, State& next, TokenType& type, std::string& text)
{
	State needed = State::RuleIdentifier | State::RuleLine;
	if ((current & needed) == State::Invalid)
	{
		return 0;
	}

	if (current == State::RuleIdentifier && StartsWith(data, "line"))
	{
		next = State::RuleLine;
		type = TokenType::Retry;
		text = "";
		return sizeof("line") - 1;
	}

	if (current == State::RuleLine)
	{
		next = State::RuleIdentifier;
		type = TokenType::Action;
		size_t size = data.find_first_of(" \t\r\n");
		text = data.substr(0, size);
		return size;
	}

	return 0;
}

size_t Error(std::string_view data, State current, State& next, TokenType& type, std::string& text)
{
	next = current;
	type = TokenType::Unknown;
	text = std::string(1, data[0]);
	return 1;
}
