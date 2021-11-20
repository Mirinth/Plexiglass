#include <string>

#include "doctest.h"

#include <error.hpp>
#include <parser/parser.hpp>
#include <test_files.hpp>


// ============================================================================
// Misc test cases
// ============================================================================

TEST_CASE("Parser: Empty file")
{
    std::filesystem::path path = GetTestRoot() / "parser/empty-file.txt";
    CHECK_THROWS_WITH_AS(Parse(path),
                         "Error on line 1: Expected keyword found eof",
                         PlexiException);
}

TEST_CASE("Parser: No keyword at start")
{
    std::filesystem::path path =
        GetTestRoot() / "parser/no-keyword-at-start.txt";
    CHECK_THROWS_WITH_AS(Parse(path),
                         "Error on line 2: Unrecognized keyword junk",
                         PlexiException);
}

// ============================================================================
// Expression test cases
// ============================================================================

TEST_CASE("Parser: Expression with no identifier")
{
    std::filesystem::path path =
        GetTestRoot() / "parser/expression-with-no-identifier.txt";
    CHECK_THROWS_WITH_AS(Parse(path),
                         "Error on line 2: Expected identifier found eof",
                         PlexiException);
}

TEST_CASE("Parser: Expression with no indent")
{
    std::filesystem::path path =
        GetTestRoot() / "parser/expression-with-no-indent.txt";
    CHECK_THROWS_WITH_AS(Parse(path),
                         "Error on line 2: Expected indent found eof",
                         PlexiException);
}

TEST_CASE("Parser: Expression with invalid regex")
{
    std::filesystem::path path =
        GetTestRoot() / "parser/expression-with-bad-regex.txt";
    CHECK_THROWS_WITH_AS(Parse(path), "Error on line 3: Malformed regex.",
                         PlexiException);
}

TEST_CASE("Parser: Expression with extra stuff on first line")
{
    std::filesystem::path path =
        GetTestRoot() / "parser/expression-long-first-line.txt";
    CHECK_THROWS_WITH_AS(Parse(path),
                         "Error on line 2: Expected indent found text junk",
                         PlexiException);
}

// ============================================================================
// Pattern test cases
// ============================================================================

TEST_CASE("Parser: Pattern with no name")
{
    std::filesystem::path path =
        GetTestRoot() / "parser/pattern-with-no-name.txt";
    CHECK_THROWS_WITH_AS(Parse(path),
                         "Error on line 2: Expected identifier found eof",
                         PlexiException);
}

TEST_CASE("Parser: Empty pattern")
{
    std::filesystem::path path = GetTestRoot() / "parser/pattern-empty.txt";
    CHECK_THROWS_WITH_AS(Parse(path),
                         "Error on line 2: Expected indent found eof",
                         PlexiException);
}

TEST_CASE("Parser: Pattern with extra stuff on first line")
{
    std::filesystem::path path = GetTestRoot() / "parser/pattern-long-line.txt";
    CHECK_THROWS_WITH_AS(Parse(path),
                         "Error on line 2: Expected indent found text junk",
                         PlexiException);
}

TEST_CASE("Parser: Pattern with alternator at start")
{
    std::filesystem::path path =
        GetTestRoot() / "parser/pattern-with-alternator-at-start.txt";
    CHECK_THROWS_WITH_AS(
        Parse(path), "Error on line 3: Expected identifier found alternator |",
        PlexiException);
}

TEST_CASE("Parser: Pattern with alternator at end")
{
    std::filesystem::path path =
        GetTestRoot() / "parser/pattern-with-alternator-at-end.txt";
    CHECK_THROWS_WITH_AS(Parse(path),
                         "Error on line 4: Expected identifier found eof",
                         PlexiException);
}

TEST_CASE("Parser: Pattern with duplicate alternator")
{
    std::filesystem::path path =
        GetTestRoot() / "parser/pattern-with-duplicate-alternator.txt";
    CHECK_THROWS_WITH_AS(Parse(path),
                         "Error on line 3: Expected 'expression', 'pattern', "
                         "or 'rule' found alternator |",
                         PlexiException);
}

// ============================================================================
// Rule test cases
// ============================================================================

TEST_CASE("Parser: Rule with no identifier")
{
    std::filesystem::path path =
        GetTestRoot() / "parser/rule-with-no-identifier.txt";
    CHECK_THROWS_WITH_AS(Parse(path),
                         "Error on line 2: Expected identifier found eof",
                         PlexiException);
}

TEST_CASE("Parser: Rule with no action")
{
    std::filesystem::path path =
        GetTestRoot() / "parser/rule-with-no-action.txt";
    CHECK_THROWS_WITH_AS(Parse(path),
                         "Error on line 2: Expected indent found eof",
                         PlexiException);
}

TEST_CASE("Parser: Rule with extra stuff on first line")
{
    std::filesystem::path path =
        GetTestRoot() / "parser/rule-long-first-line.txt";
    CHECK_THROWS_WITH_AS(Parse(path),
                         "Error on line 2: Expected indent found text junk",
                         PlexiException);
}

TEST_CASE("Parser: Rule with bad action")
{
    std::filesystem::path path =
        GetTestRoot() / "parser/rule-with-bad-action.txt";
    CHECK_THROWS_WITH_AS(Parse(path),
                         "Error on line 3: Expected action found text junk",
                         PlexiException);
}

TEST_CASE("Parser: Rule with produce at end of file")
{
    std::filesystem::path path =
        GetTestRoot() / "parser/rule-with-produce-at-eof.txt";
    CHECK_THROWS_WITH_AS(
        Parse(path), "Error on line 3: Expected identifier before end of line",
        PlexiException);
}

TEST_CASE("Parser: Rule with empty produce")
{
    std::filesystem::path path =
        GetTestRoot() / "parser/rule-with-empty-produce.txt";
    CHECK_THROWS_WITH_AS(
        Parse(path), "Error on line 3: Expected identifier before end of line",
        PlexiException);
}

TEST_CASE("Parser: Rule with transition at eof")
{
    std::filesystem::path path =
        GetTestRoot() / "parser/rule-with-transition-at-eof.txt";
    CHECK_THROWS_WITH_AS(
        Parse(path), "Error on line 3: Expected identifier before end of line",
        PlexiException);
}

TEST_CASE("Parser: Rule with empty transition")
{
    std::filesystem::path path =
        GetTestRoot() / "parser/rule-with-empty-transition.txt";
    CHECK_THROWS_WITH_AS(
        Parse(path), "Error on line 3: Expected identifier before end of line",
        PlexiException);
}
