#include <sstream>
#include <string>

#include "doctest.h"

#include <lexer/lexer.hpp>
#include <lexer/token.hpp>

#include "test_files.hpp"

TEST_CASE("Lexer: Test the lexer")
{
    std::stringstream out;
    std::string input = ReadTestFile("lexer/lexer-test-in.txt");
    std::string base = ReadTestFile("lexer/lexer-test-base.txt");

    Lexer lexer(input);

    while (lexer.PeekToken() != TokenType::Eof)
    {
        Token tok(lexer.PeekLine(), lexer.PeekToken(), lexer.PeekText());
        out << tok.ToString() << "\n";
        lexer.Shift();
    }

    CHECK(base == out.str());
}