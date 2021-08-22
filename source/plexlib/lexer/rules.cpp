#include <lexer/rules.hpp>

std::vector<Rule> Rules = {
	&Newline,
	&Indent,
	&Whitespace,
	&Comment,
	&Keyword,
	&End, // needs to be before Identifier
	&PatternAlternator, // needs to be before Identifier
	&Action, // needs to be before Identifier
	&Identifier,
	&ExpressionPattern,
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
	State needed = State::ExpressionKeyword | State::PatternKeyword | State::RuleKeyword | State::PatternIdentifier;
	if ((current & needed) == State::Invalid)
	{
		return 0;
	}

	if (current == State::ExpressionKeyword)
	{
		next = State::ExpressionIdentifier;
	}
	else if (current == State::PatternKeyword)
	{
		next = State::PatternIdentifier;
	}
	else if (current == State::RuleKeyword)
	{
		next = State::RuleIdentifier;
	}
	else if (current == State::PatternIdentifier)
	{
		next = current;
	}

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
