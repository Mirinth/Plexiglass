#include <lexer/rules.hpp>

#include <map>

std::vector<Matcher> Matchers = {
	&Newline,
	&Indent,
	&Whitespace,
	&Comment,
	&Keyword,
	&End,
	&PatternAlternator,
	&Action,
	&LineAction,
	&LineMulti,
	&ExpressionPattern,
	// Identifier needs to come after everything but the error rule since it
	// interferes with everything that follows it.
	&Identifier,
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

size_t Keyword(std::string_view data, State current, State& next, TokenType& type, std::string& text)
{
	if ((current & State::Initial) == State::Invalid)
	{
		return 0;
	}

	static std::vector<std::tuple<const char* const, size_t, State>> keywords = {
		{ "expression", sizeof("expression") -1 , State::ExpressionKeyword },
		{ "pattern", sizeof("pattern") - 1, State::PatternKeyword },
		{ "rule", sizeof("rule") - 1, State::RuleKeyword },
	};

	for (auto& [keyword, size, state] : keywords)
	{
		if (StartsWith(data, keyword))
		{
			next = state;
			type = TokenType::Keyword;
			text = keyword;
			return size;
		}
	}

	return 0;
}

size_t End(std::string_view data, State current, State& next, TokenType& type, std::string& text)
{
	State needed = State::PatternIdentifier | State::RuleIdentifier;
	if ((current & needed) == State::Invalid)
	{
		return 0;
	}

	if (data[0] != ';')
	{
		return 0;
	}

	next = State::Initial;
	type = TokenType::End;
	text = ";";
	return 1;
}

size_t Identifier(std::string_view data, State current, State& next, TokenType& type, std::string& text)
{
	static std::map<State, State> nextState = {
		{State::ExpressionKeyword, State::ExpressionIdentifier},
		{State::PatternKeyword, State::PatternIdentifier},
		{State::RuleKeyword, State::RuleIdentifier},
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

size_t Newline(std::string_view data, State current, State& next, TokenType& type, std::string& text)
{
	if (data[0] != '\n')
	{
		return 0;
	}

	next = current | State::StartOfLine;
	type = TokenType::Newline;
	text = "\\n";
	return 1;
}

size_t Indent(std::string_view data, State current, State& next, TokenType& type, std::string& text)
{
	if ((current & State::StartOfLine) == State::Invalid)
	{
		return 0;
	}

	if (data[0] != '\t' && !StartsWith(data, "    "))
	{
		return 0;
	}

	next = current & (~State::StartOfLine);
	type = TokenType::Indent;
	text = "\\t";

	return data[0] == '\t' ? 1 : 4;
}

size_t Whitespace(std::string_view data, State current, State& next, TokenType& type, std::string& text)
{
	if (data[0] != ' ' && data[0] != '\t')
	{
		return 0;
	}

	next = current;
	type = TokenType::Retry;
	return data.find_first_not_of(" \t");
}

size_t Comment(std::string_view data, State current, State& next, TokenType& type, std::string& text)
{
	if (data[0] != '#')
	{
		return 0;
	}

	next = current;
	type = TokenType::Retry;
	return data.find_first_of('\n');
}

size_t ExpressionPattern(std::string_view data, State current, State& next, TokenType& type, std::string& text)
{
	if ((current & State::ExpressionIdentifier) == State::Invalid)
	{
		return 0;
	}

	next = State::Initial;
	type = TokenType::Expression;

	size_t size = data.find_first_of("\r\n");
	std::string_view substr = data.substr(0, size);
	text = std::string(substr);

	if (text.size() > 1 && text[0] == '\\' && text[1] == ' ' || text[1] == '\t')
	{
		text.erase(0, 1);
	}

	return size;
}

size_t PatternAlternator(std::string_view data, State current, State& next, TokenType& type, std::string& text)
{
	if ((current & State::PatternIdentifier) == State::Invalid)
	{
		return 0;
	}

	if (data[0] != '|')
	{
		return 0;
	}

	next = current;
	type = TokenType::Alternator;
	text = "|";
	return 1;
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
