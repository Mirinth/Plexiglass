#include <string>

#include <error.hpp>
#include <parser/parser.hpp>

#include "doctest.h"

// ============================================================================
// Misc test cases
// ============================================================================

TEST_CASE("Parser: Empty file")
{
    CHECK_THROWS_WITH_AS(Parse("parser/empty-file.txt"),
                         "Error on line 1: Expected keyword found eof",
                         PlexiException);
}

TEST_CASE("Parser: No keyword at start")
{
    CHECK_THROWS_WITH_AS(Parse("parser/no-keyword-at-start.txt"),
                         "Error on line 2: Unrecognized keyword junk",
                         PlexiException);
}

// ============================================================================
// Expression test cases
// ============================================================================

TEST_CASE("Parser: Expression with no identifier")
{
    CHECK_THROWS_WITH_AS(Parse("parser/expression-with-no-identifier.txt"),
                         "Error on line 2: Expected identifier found eof",
                         PlexiException);
}

TEST_CASE("Parser: Expression with no indent")
{
    CHECK_THROWS_WITH_AS(Parse("parser/expression-with-no-indent.txt"),
                         "Error on line 2: Expected indent found eof",
                         PlexiException);
}

TEST_CASE("Parser: Expression with invalid regex")
{
    CHECK_THROWS_WITH_AS(Parse("parser/expression-with-bad-regex.txt"),
                         "Error on line 3: Malformed regex.", PlexiException);
}

TEST_CASE("Parser: Expression with extra stuff on first line")
{
    CHECK_THROWS_WITH_AS(Parse("parser/expression-long-first-line.txt"),
                         "Error on line 2: Expected indent found text junk",
                         PlexiException);
}

// ============================================================================
// Pattern test cases
// ============================================================================

TEST_CASE("Parser: Pattern with no name")
{
    CHECK_THROWS_WITH_AS(Parse("parser/pattern-with-no-name.txt"),
                         "Error on line 2: Expected identifier found eof",
                         PlexiException);
}

TEST_CASE("Parser: Empty pattern")
{
    CHECK_THROWS_WITH_AS(Parse("parser/pattern-empty.txt"),
                         "Error on line 2: Expected indent found eof",
                         PlexiException);
}

TEST_CASE("Parser: Pattern with extra stuff on first line")
{
    CHECK_THROWS_WITH_AS(Parse("parser/pattern-long-line.txt"),
                         "Error on line 2: Expected indent found text junk",
                         PlexiException);
}

TEST_CASE("Parser: Pattern with alternator at start")
{
    CHECK_THROWS_WITH_AS(
        Parse("parser/pattern-with-alternator-at-start.txt"),
        "Error on line 3: Expected identifier found alternator |",
        PlexiException);
}

TEST_CASE("Parser: Pattern with alternator at end")
{
    CHECK_THROWS_WITH_AS(Parse("parser/pattern-with-alternator-at-end.txt"),
                         "Error on line 4: Expected identifier found eof",
                         PlexiException);
}

TEST_CASE("Parser: Pattern with duplicate alternator")
{
    CHECK_THROWS_WITH_AS(Parse("parser/pattern-with-duplicate-alternator.txt"),
                         "Error on line 3: Expected 'expression', 'pattern', "
                         "or 'rule' found alternator |",
                         PlexiException);
}

// ============================================================================
// Rule test cases
// ============================================================================

TEST_CASE("Parser: Rule with no identifier")
{
    CHECK_THROWS_WITH_AS(Parse("parser/rule-with-no-identifier.txt"),
                         "Error on line 2: Expected identifier found eof",
                         PlexiException);
}

TEST_CASE("Parser: Rule with no action")
{
    CHECK_THROWS_WITH_AS(Parse("parser/rule-with-no-action.txt"),
                         "Error on line 2: Expected indent found eof",
                         PlexiException);
}

TEST_CASE("Parser: Rule with extra stuff on first line")
{
    CHECK_THROWS_WITH_AS(Parse("parser/rule-long-first-line.txt"),
                         "Error on line 2: Expected indent found text junk",
                         PlexiException);
}

TEST_CASE("Parser: Rule with bad action")
{
    CHECK_THROWS_WITH_AS(Parse("parser/rule-with-bad-action.txt"),
                         "Error on line 3: Expected action found text junk",
                         PlexiException);
}

TEST_CASE("Parser: Rule with produce at end of file")
{
    CHECK_THROWS_WITH_AS(
        Parse("parser/rule-with-produce-at-eof.txt"),
        "Error on line 3: Expected identifier before end of line",
        PlexiException);
}

TEST_CASE("Parser: Rule with empty produce")
{
    CHECK_THROWS_WITH_AS(
        Parse("parser/rule-with-empty-produce.txt"),
        "Error on line 3: Expected identifier before end of line",
        PlexiException);
}

TEST_CASE("Parser: Rule with transition at eof")
{
    CHECK_THROWS_WITH_AS(
        Parse("parser/rule-with-transition-at-eof.txt"),
        "Error on line 3: Expected identifier before end of line",
        PlexiException);
}

TEST_CASE("Parser: Rule with empty transition")
{
    CHECK_THROWS_WITH_AS(
        Parse("parser/rule-with-empty-transition.txt"),
        "Error on line 3: Expected identifier before end of line",
        PlexiException);
}
