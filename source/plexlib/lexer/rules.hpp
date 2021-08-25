#pragma once

#include <functional>
#include <string>
#include <string_view>
#include <vector>

#include <lexer/token.hpp>

enum class State
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

	Any = ~0,
};

typedef std::function<size_t(std::string_view)> Matcher;
typedef std::tuple<State, Matcher, State, TokenType> Rule;
extern std::vector<Rule> Rules;

size_t Comment(std::string_view data);
size_t Whitespace(std::string_view data);

size_t Identifier(std::string_view data);

size_t Regex(std::string_view data);

size_t MultilineEnd(std::string_view data);

size_t Error(std::string_view data);

Matcher Literal(std::string value);
