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

	StartOfLine = 1024,

	Any = ~0,
};

typedef std::tuple<size_t, std::string> MatcherResult;
typedef std::function<MatcherResult(std::string_view)> Matcher;
typedef std::tuple<State, Matcher, State, TokenType> Rule;
extern std::vector<Rule> Rules;

State operator|(State left, State right);
State operator&(State left, State right);
State operator~(State s);

MatcherResult Comment(std::string_view data);
MatcherResult Whitespace(std::string_view data);

MatcherResult Identifier(std::string_view data);

MatcherResult Regex(std::string_view data);

MatcherResult MultilineEnd(std::string_view data);

MatcherResult Error(std::string_view data);

Matcher Literal(std::string value);
Matcher Literal(std::string match, std::string produce);
