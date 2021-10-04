#include <sstream>
#include <string>

#include "doctest.h"

#include <lexer/lexer.hpp>
#include <lexer/token.hpp>

#include "read_test_file.hpp"

TEST_CASE("Test the lexer")
{
    std::stringstream out;
    std::string input = ReadTestFile("lexer/lexer-test-in.txt");
    std::string base = ReadTestFile("lexer/lexer-test-base.txt");

    Lexer lexer(input);

    while (lexer.Peek().type != TokenType::Eof)
    {
        out << lexer.Peek().ToString() << "\n";
        lexer.Shift();
    }

    CHECK(base == out.str());
}