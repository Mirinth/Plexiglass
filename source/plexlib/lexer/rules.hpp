#pragma once

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
};

//State operator|(State left, State right);
//State operator&(State left, State right);
//State operator!(State s);

typedef size_t(*Rule)(std::string_view, State, State&, TokenType&, std::string&);

size_t Newline(std::string_view data, State current, State& next, TokenType& type, std::string& text);
size_t Indent(std::string_view data, State current, State& next, TokenType& type, std::string& text);

size_t Keyword(std::string_view data, std::string keyword, State possibleNext, State current, State& next, TokenType& type, std::string& text);
size_t KeywordExpression(std::string_view data, State current, State& next, TokenType& type, std::string& text);

extern std::vector<Rule> Rules;
