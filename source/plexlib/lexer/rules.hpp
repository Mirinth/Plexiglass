#pragma once

#include <functional>
#include <string>
#include <string_view>
#include <vector>

#include <lexer/token.hpp>

typedef std::function<size_t(std::string_view)> Matcher;
typedef std::tuple<Matcher, TokenType> Rule;
extern std::vector<Rule> Rules;

size_t Whitespace(std::string_view data);

size_t Identifier(std::string_view data);

size_t Regex(std::string_view data);

size_t Error(std::string_view data);

Matcher Literal(std::string value);
