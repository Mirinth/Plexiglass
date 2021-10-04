#include <string>

#include <error.hpp>
#include <parser/parser.hpp>

#include "doctest.h"

#include "read_test_file.hpp"

// ============================================================================
// Misc test cases
// ============================================================================

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

// ============================================================================
// Expression test cases
// ============================================================================

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
    CHECK_THROWS_WITH_AS(Parse(data), "Error on line 3: Malformed regex.",
                         PlexiException);
}

TEST_CASE("Parser: Expression with extra stuff on first line")
{
    std::string data = ReadTestFile("parser/expression-long-first-line.txt");
    CHECK_THROWS_WITH_AS(Parse(data),
                         "Error on line 2: Expected indent found text junk",
                         PlexiException);
}

// ============================================================================
// Pattern test cases
// ============================================================================

TEST_CASE("Parser: Pattern with no name")
{
    std::string data = ReadTestFile("parser/pattern-with-no-name.txt");
    CHECK_THROWS_WITH_AS(Parse(data),
                         "Error on line 2: Expected identifier found eof",
                         PlexiException);
}

TEST_CASE("Parser: Empty pattern")
{
    std::string data = ReadTestFile("parser/pattern-empty.txt");
    CHECK_THROWS_WITH_AS(Parse(data),
                         "Error on line 2: Expected indent found eof",
                         PlexiException);
}

TEST_CASE("Parser: Pattern with extra stuff on first line")
{
    std::string data = ReadTestFile("parser/pattern-long-line.txt");
    CHECK_THROWS_WITH_AS(Parse(data),
                         "Error on line 2: Expected indent found text junk",
                         PlexiException);
}

TEST_CASE("Parser: Pattern with alternator at start")
{
    std::string data =
        ReadTestFile("parser/pattern-with-alternator-at-start.txt");
    CHECK_THROWS_WITH_AS(
        Parse(data), "Error on line 3: Expected identifier found alternator |",
        PlexiException);
}

TEST_CASE("Parser: Pattern with alternator at end")
{
    std::string data =
        ReadTestFile("parser/pattern-with-alternator-at-end.txt");
    CHECK_THROWS_WITH_AS(Parse(data),
                         "Error on line 4: Expected identifier found eof",
                         PlexiException);
}

TEST_CASE("Parser: Pattern with duplicate alternator")
{
    std::string data =
        ReadTestFile("parser/pattern-with-duplicate-alternator.txt");
    CHECK_THROWS_WITH_AS(Parse(data),
                         "Error on line 3: Expected 'expression', 'pattern', "
                         "or 'rule' found alternator |",
                         PlexiException);
}

// ============================================================================
// Rule test cases
// ============================================================================

TEST_CASE("Parser: Rule with no identifier")
{
    std::string data = ReadTestFile("parser/rule-with-no-identifier.txt");
    CHECK_THROWS_WITH_AS(Parse(data),
                         "Error on line 2: Expected identifier found eof",
                         PlexiException);
}

TEST_CASE("Parser: Rule with no action")
{
    std::string data = ReadTestFile("parser/rule-with-no-action.txt");
    CHECK_THROWS_WITH_AS(Parse(data),
                         "Error on line 2: Expected indent found eof",
                         PlexiException);
}

TEST_CASE("Parser: Rule with extra stuff on first line")
{
    std::string data = ReadTestFile("parser/rule-long-first-line.txt");
    CHECK_THROWS_WITH_AS(Parse(data),
                         "Error on line 2: Expected indent found text junk",
                         PlexiException);
}

TEST_CASE("Parser: Rule with bad action")
{
    std::string data = ReadTestFile("parser/rule-with-bad-action.txt");
    CHECK_THROWS_WITH_AS(Parse(data),
                         "Error on line 3: Expected action found text junk",
                         PlexiException);
}

TEST_CASE("Parser: Rule with produce at end of file")
{
    std::string data = ReadTestFile("parser/rule-with-produce-at-eof.txt");
    CHECK_THROWS_WITH_AS(
        Parse(data), "Error on line 3: Expected identifier before end of line",
        PlexiException);
}

TEST_CASE("Parser: Rule with empty produce")
{
    std::string data = ReadTestFile("parser/rule-with-empty-produce.txt");
    CHECK_THROWS_WITH_AS(
        Parse(data), "Error on line 3: Expected identifier before end of line",
        PlexiException);
}

TEST_CASE("Parser: Rule with transition at eof")
{
    std::string data = ReadTestFile("parser/rule-with-transition-at-eof.txt");
    CHECK_THROWS_WITH_AS(
        Parse(data), "Error on line 3: Expected identifier before end of line",
        PlexiException);
}

TEST_CASE("Parser: Rule with empty transition")
{
    std::string data = ReadTestFile("parser/rule-with-empty-transition.txt");
    CHECK_THROWS_WITH_AS(
        Parse(data), "Error on line 3: Expected identifier before end of line",
        PlexiException);
}
