#include <sstream>
#include <string>

#include "doctest.h"

#include <lexer/lexer.hpp>

#include "test_files.hpp"

TEST_CASE("Lexer: Test the lexer")
{
    std::stringstream out;
    std::string input = ReadTestFile("lexer/lexer-test-in.txt");
    std::string base = ReadTestFile("lexer/lexer-test-base.txt");

    Lexer lexer(input);

    while (lexer.PeekToken() != TokenType::Eof)
    {
        out << ToString(lexer.PeekToken(), lexer.PeekText()) << "\n";
        lexer.Shift();
    }

    CHECK(base == out.str());
}