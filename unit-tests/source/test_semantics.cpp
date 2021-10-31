#include "doctest.h"

#include <analyzer/analyzer.hpp>
#include <error.hpp>
#include <parser/parser.hpp>
#include <parser/tree.hpp>

#include "test_files.hpp"

TEST_CASE("Semantics: Expression with duplicate name")
{
    std::string data = ReadTestFile("semantics/expression-duplicate-name.txt");
    FileNode file = Parse(data);

    CHECK_THROWS_WITH_AS(Analyze(file),
                         "Error on line 5: 'name' already defined on line 2",
                         PlexiException);
}

TEST_CASE("Semantics: Pattern with duplicate name")
{
    std::string data = ReadTestFile("semantics/pattern-duplicate-name.txt");
    FileNode file = Parse(data);

    CHECK_THROWS_WITH_AS(Analyze(file),
                         "Error on line 5: 'name' already defined on line 2",
                         PlexiException);
}

TEST_CASE("Semantics: Pattern with name that duplicates an expression")
{
    std::string data =
        ReadTestFile("semantics/pattern-expression-name-conflict.txt");
    FileNode file = Parse(data);

    CHECK_THROWS_WITH_AS(Analyze(file),
                         "Error on line 5: 'name' already defined on line 2",
                         PlexiException);
}

TEST_CASE("Semantics: Expression with name that duplicates a pattern")
{
    std::string data =
        ReadTestFile("semantics/expression-pattern-name-conflict.txt");
    FileNode file = Parse(data);

    CHECK_THROWS_WITH_AS(Analyze(file),
                         "Error on line 5: 'name' already defined on line 2",
                         PlexiException);
}

TEST_CASE("Semantics: Rule that uses an undefined name")
{
    std::string data = ReadTestFile("semantics/rule-name-undefined.txt");
    FileNode file = Parse(data);

    CHECK_THROWS_WITH_AS(Analyze(file),
                         "Error on line 2: Undefined name 'name'",
                         PlexiException);
}

TEST_CASE("Semantics: Pattern that uses an undefined name")
{
    std::string data = ReadTestFile("semantics/pattern-name-undefined.txt");
    FileNode file = Parse(data);

    CHECK_THROWS_WITH_AS(Analyze(file), "Error on line 3: Undefined name 'abc'",
                         PlexiException);
}

TEST_CASE("Semantics: Pattern that uses an undefined name far in")
{
    std::string data =
        ReadTestFile("semantics/pattern-name-undefined-far-in.txt");
    FileNode file = Parse(data);

    CHECK_THROWS_WITH_AS(Analyze(file),
                         "Error on line 9: Undefined name 'missing'",
                         PlexiException);
}

TEST_CASE("Semantics: Rule that uses rewind")
{
    std::string data = ReadTestFile("semantics/rule-using-rewind.txt");
    FileNode file = Parse(data);

    CHECK_THROWS_WITH_AS(Analyze(file),
                         "Error on line 6: 'rewind' action not yet supported",
                         PlexiException);
}

TEST_CASE("Semantics: Rule that uses a transition")
{
    std::string data = ReadTestFile("semantics/rule-using-transition.txt");
    FileNode file = Parse(data);

    CHECK_THROWS_WITH_AS(
        Analyze(file), "Error on line 6: 'transition' action not yet supported",
        PlexiException);
}

TEST_CASE("Semantics: Reject pattern statements")
{
    std::string data = ReadTestFile("semantics/reject-pattern-statements.txt");
    FileNode file = Parse(data);

    CHECK_THROWS_WITH_AS(
        Analyze(file), "Error on line 5: 'pattern' statement not yet supported",
        PlexiException);
}

TEST_CASE("Semantics: Reject duplicate produce-nothing")
{
    std::string data =
        ReadTestFile("semantics/rule-duplicate-produce-nothing.txt");
    FileNode file = Parse(data);

    CHECK_THROWS_WITH_AS(
        Analyze(file), "Error on line 7: `produce-nothing` already used in rule on line 6",
        PlexiException);
}

TEST_CASE("Semantics: Reject duplicate produce")
{
    std::string data =
        ReadTestFile("semantics/rule-duplicate-produce.txt");
    FileNode file = Parse(data);

    CHECK_THROWS_WITH_AS(Analyze(file),
                         "Error on line 7: `produce` already used in rule on line 6",
                         PlexiException);
}

TEST_CASE("Semantics: Reject duplicate rewind")
{
    std::string data = ReadTestFile("semantics/rule-duplicate-rewind.txt");
    FileNode file = Parse(data);

    CHECK_THROWS_WITH_AS(
        Analyze(file),
        "Error on line 7: `rewind` already used in rule on line 6",
        PlexiException);
}

TEST_CASE("Semantics: Reject duplicate transition")
{
    std::string data = ReadTestFile("semantics/rule-duplicate-transition.txt");
    FileNode file = Parse(data);

    CHECK_THROWS_WITH_AS(
        Analyze(file),
        "Error on line 7: `transition` already used in rule on line 6",
        PlexiException);
}

TEST_CASE("Semantics: Reject duplicate increment")
{
    std::string data = ReadTestFile("semantics/rule-duplicate-increment.txt");
    FileNode file = Parse(data);

    CHECK_THROWS_WITH_AS(
        Analyze(file),
        "Error on line 7: `++line` already used in rule on line 6",
        PlexiException);
}

TEST_CASE("Semantics: Reject mismatched duplicate increment")
{
    std::string data =
        ReadTestFile("semantics/rule-mismatched-duplicate-increment.txt");
    FileNode file = Parse(data);

    CHECK_THROWS_WITH_AS(
        Analyze(file),
        "Error on line 7: `line++` already used in rule on line 6",
        PlexiException);
}
