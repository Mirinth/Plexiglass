#pragma once

#include <iostream>
#include <string>
#include <vector>

/// <summary>
/// Returned when there are no errors.
/// </summary>
constexpr int success = 0;
/// <summary>
/// Returned when the command line parameters are incorrect.
/// </summary>
constexpr int bad_usage = 1;
/// <summary>
/// Returned when help is invoked.
/// </summary>
constexpr int help_invoked = 2;
/// <summary>
/// Returned when the input file is unreadable.
/// </summary>
constexpr int unreadable_file = 3;
/// <summary>
/// Returned when the lexer would have an illegal name.
/// </summary>
constexpr int bad_lexer_name = 4;
/// <summary>
/// Returned when there was an eror in the lexer,
/// e.g. bad syntax.
/// </summary>
constexpr int bad_lexer = 5;
/// <summary>
/// Returned when an unknown error occurs.
/// </summary>
constexpr int unknown_fault = 99;

int PlexMain(std::vector<std::string>& args,
             std::ostream& out,
             std::ostream& err);

void PrintUsage(std::ostream& out);
