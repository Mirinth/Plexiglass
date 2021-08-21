#include <lexer/rules.hpp>

std::vector<Rule> Rules = {
	&Newline,
	&Indent,
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

State operator!(State s)
{
	auto st = static_cast<std::underlying_type<State>::type>(s);
	return static_cast<State>(!st);
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

	next = next & (!State::StartOfLine);
	type = TokenType::Indent;
	text = "\\t";

	return data[0] == '\t' ? 1 : 4;
}
