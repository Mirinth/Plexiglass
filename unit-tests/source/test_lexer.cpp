#include <sstream>
#include <string>

#include "doctest.h"

#include <lexer/lexer.hpp>

#include "test_files.hpp"

TEST_CASE("Lexer: Test the lexer")
{
    std::stringstream out;
    std::filesystem::path path = GetTestRoot() / "lexer/lexer-test-in.txt";
    std::string base = ReadTestFile("lexer/lexer-test-base.txt");

    Lexer lexer(path);

    while (lexer.PeekToken() != TokenType::Eof)
    {
        out << lexer.PeekLine() << ": "
            << ToString(lexer.PeekToken(), lexer.PeekText()) << "\n";
        lexer.Shift();
    }

    CHECK(base == out.str());
}