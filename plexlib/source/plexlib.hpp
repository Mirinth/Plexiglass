#pragma once

#include <iostream>
#include <string>
#include <vector>

constexpr int success = 0;

constexpr int bad_usage = 1;
constexpr int help_invoked = 2;

constexpr int unreadable_file = 3;
constexpr int bad_lexer_name = 4;

constexpr int bad_lexer = 5;

constexpr int unknown_fault = 99;

int PlexMain(std::vector<std::string>& args,
             std::ostream& out,
             std::ostream& err);

void PrintUsage(std::ostream& out);
