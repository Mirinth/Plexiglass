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
typedef std::function<MatcherResult(std::string_view)> Matcher2;
typedef std::tuple<State, Matcher2, State, TokenType> Rule;
extern std::vector<Rule> Rules;

State operator|(State left, State right);
State operator&(State left, State right);
State operator~(State s);

MatcherResult Newline(std::string_view data);
MatcherResult Comment(std::string_view data);
MatcherResult Indent(std::string_view data);
MatcherResult Whitespace(std::string_view data);

MatcherResult Identifier(std::string_view data);

MatcherResult ExpressionKeyword(std::string_view data);
MatcherResult Regex(std::string_view data);

MatcherResult PatternKeyword(std::string_view data);

MatcherResult RuleKeyword(std::string_view data);

size_t OldIdentifier(std::string_view data, State current, State& next, TokenType& type, std::string& text);
size_t End(std::string_view data, State current, State& next, TokenType& type, std::string& text);

typedef std::function<size_t(std::string_view, State, State&, TokenType&, std::string&)> Matcher;

size_t PatternAlternator(std::string_view data, State current, State& next, TokenType& type, std::string& text);

size_t Action(std::string_view data, State current, State& next, TokenType& type, std::string& text);
size_t LineAction(std::string_view data, State current, State& next, TokenType& type, std::string& text);
size_t LineMulti(std::string_view data, State current, State& next, TokenType& type, std::string& text);

size_t Error(std::string_view data, State current, State& next, TokenType& type, std::string& text);

extern std::vector<Matcher> Matchers;
