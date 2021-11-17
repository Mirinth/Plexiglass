#include "doctest.h"

#include <analyzer/analyzer.hpp>
#include <error.hpp>
#include <parser/parser.hpp>
#include <parser/tree.hpp>

TEST_CASE("Semantics: Expression with duplicate name")
{
    FileNode file = Parse("semantics/expression-duplicate-name.txt");

    CHECK_THROWS_WITH_AS(Analyze(file),
                         "Error on line 5: 'name' already defined on line 2",
                         PlexiException);
}

TEST_CASE("Semantics: Pattern with duplicate name")
{
    FileNode file = Parse("semantics/pattern-duplicate-name.txt");

    CHECK_THROWS_WITH_AS(Analyze(file),
                         "Error on line 5: 'name' already defined on line 2",
                         PlexiException);
}

TEST_CASE("Semantics: Pattern with name that duplicates an expression")
{
    FileNode file = Parse("semantics/pattern-expression-name-conflict.txt");

    CHECK_THROWS_WITH_AS(Analyze(file),
                         "Error on line 5: 'name' already defined on line 2",
                         PlexiException);
}

TEST_CASE("Semantics: Expression with name that duplicates a pattern")
{
    FileNode file = Parse("semantics/expression-pattern-name-conflict.txt");

    CHECK_THROWS_WITH_AS(Analyze(file),
                         "Error on line 5: 'name' already defined on line 2",
                         PlexiException);
}

TEST_CASE("Semantics: Rule that uses an undefined name")
{
    FileNode file = Parse("semantics/rule-name-undefined.txt");

    CHECK_THROWS_WITH_AS(Analyze(file),
                         "Error on line 2: Undefined name 'name'",
                         PlexiException);
}

TEST_CASE("Semantics: Pattern that uses an undefined name")
{
    FileNode file = Parse("semantics/pattern-name-undefined.txt");

    CHECK_THROWS_WITH_AS(Analyze(file), "Error on line 3: Undefined name 'abc'",
                         PlexiException);
}

TEST_CASE("Semantics: Pattern that uses an undefined name far in")
{
    FileNode file = Parse("semantics/pattern-name-undefined-far-in.txt");

    CHECK_THROWS_WITH_AS(Analyze(file),
                         "Error on line 9: Undefined name 'missing'",
                         PlexiException);
}

TEST_CASE("Semantics: Rule that uses rewind")
{
    FileNode file = Parse("semantics/rule-using-rewind.txt");

    CHECK_THROWS_WITH_AS(Analyze(file),
                         "Error on line 6: 'rewind' action not yet supported",
                         PlexiException);
}

TEST_CASE("Semantics: Rule that uses a transition")
{
    FileNode file = Parse("semantics/rule-using-transition.txt");

    CHECK_THROWS_WITH_AS(
        Analyze(file), "Error on line 6: 'transition' action not yet supported",
        PlexiException);
}

TEST_CASE("Semantics: Reject pattern statements")
{
    FileNode file = Parse("semantics/reject-pattern-statements.txt");

    CHECK_THROWS_WITH_AS(
        Analyze(file), "Error on line 5: 'pattern' statement not yet supported",
        PlexiException);
}

TEST_CASE("Semantics: Reject duplicate produce-nothing")
{
    FileNode file = Parse("semantics/rule-duplicate-produce-nothing.txt");

    CHECK_THROWS_WITH_AS(
        Analyze(file),
        "Error on line 7: `produce-nothing` already used in rule on line 6",
        PlexiException);
}

TEST_CASE("Semantics: Reject duplicate produce")
{
    FileNode file = Parse("semantics/rule-duplicate-produce.txt");

    CHECK_THROWS_WITH_AS(
        Analyze(file),
        "Error on line 7: `produce` already used in rule on line 6",
        PlexiException);
}

TEST_CASE("Semantics: Reject duplicate rewind")
{
    FileNode file = Parse("semantics/rule-duplicate-rewind.txt");

    CHECK_THROWS_WITH_AS(
        Analyze(file),
        "Error on line 7: `rewind` already used in rule on line 6",
        PlexiException);
}

TEST_CASE("Semantics: Reject duplicate transition")
{
    FileNode file = Parse("semantics/rule-duplicate-transition.txt");

    CHECK_THROWS_WITH_AS(
        Analyze(file),
        "Error on line 7: `transition` already used in rule on line 6",
        PlexiException);
}

TEST_CASE("Semantics: Reject duplicate increment")
{
    FileNode file = Parse("semantics/rule-duplicate-increment.txt");

    CHECK_THROWS_WITH_AS(
        Analyze(file),
        "Error on line 7: `++line` already used in rule on line 6",
        PlexiException);
}

TEST_CASE("Semantics: Reject mismatched duplicate increment")
{
    FileNode file = Parse("semantics/rule-duplicate-mismatched-increment.txt");

    CHECK_THROWS_WITH_AS(
        Analyze(file),
        "Error on line 7: `line++` already used in rule on line 6",
        PlexiException);
}

TEST_CASE("Semantics: Reject duplicate decrement")
{
    FileNode file = Parse("semantics/rule-duplicate-decrement.txt");

    CHECK_THROWS_WITH_AS(
        Analyze(file),
        "Error on line 7: `--line` already used in rule on line 6",
        PlexiException);
}

TEST_CASE("Semantics: Reject mismatched duplicate decrement")
{
    FileNode file = Parse("semantics/rule-duplicate-mismatched-decrement.txt");

    CHECK_THROWS_WITH_AS(
        Analyze(file),
        "Error on line 7: `line--` already used in rule on line 6",
        PlexiException);
}
