#include <string>

#include <error.hpp>
#include <parser/parser.hpp>

#include "doctest.h"

#include "read_test_file.hpp"

TEST_CASE("Parser: Empty file")
{
    std::string data = ReadTestFile("parser/empty-file.txt");
    CHECK_THROWS_WITH_AS(Parse(data),
                         "Error on line 1: Expected keyword found eof",
                         PlexiException);
}

TEST_CASE("Parser: No keyword at start")
{
    std::string data = ReadTestFile("parser/no-keyword-at-start.txt");
    CHECK_THROWS_WITH_AS(Parse(data),
                         "Error on line 2: Unrecognized keyword junk",
                         PlexiException);
}

TEST_CASE("Parser: Expression with no identifier")
{
    std::string data = ReadTestFile("parser/expression-with-no-identifier.txt");
    CHECK_THROWS_WITH_AS(Parse(data),
                         "Error on line 2: Expected identifier found eof",
                         PlexiException);
}

TEST_CASE("Parser: Expression with no indent")
{
    std::string data = ReadTestFile("parser/expression-with-no-indent.txt");
    CHECK_THROWS_WITH_AS(Parse(data),
                         "Error on line 2: Expected indent found eof",
                         PlexiException);
}

TEST_CASE("Parser: Expression with invalid regex")
{
    std::string data = ReadTestFile("parser/expression-with-bad-regex.txt");
    CHECK_THROWS_WITH_AS(Parse(data),
                         "Error on line 3: Malformed regex.",
                         PlexiException);
}

TEST_CASE("Parser: Expression with extra stuff on first line")
{
    std::string data = ReadTestFile("parser/expression-long-first-line.txt");
    CHECK_THROWS_WITH_AS(Parse(data),
                         "Error on line 2: Expected indent found text junk",
                         PlexiException);
}
