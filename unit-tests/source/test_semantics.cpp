#include "doctest.h"

#include <analyzer/analyzer.hpp>
#include <error.hpp>
#include <parser/parser.hpp>
#include <parser/tree.hpp>
#include <test_files.hpp>

TEST_CASE("Semantics: Expression with duplicate name")
{
    std::filesystem::path path =
        GetTestRoot() / "semantics/expression-duplicate-name.txt";
    FileNode file = Parse(path);

    CHECK_THROWS_WITH_AS(Analyze(file),
                         "Error on line 5: 'name' already defined on line 2",
                         PlexiException);
}

TEST_CASE("Semantics: Pattern with duplicate name")
{
    std::filesystem::path path =
        GetTestRoot() / "semantics/pattern-duplicate-name.txt";
    FileNode file = Parse(path);

    CHECK_THROWS_WITH_AS(Analyze(file),
                         "Error on line 5: 'name' already defined on line 2",
                         PlexiException);
}

TEST_CASE("Semantics: Pattern with name that duplicates an expression")
{
    std::filesystem::path path =
        GetTestRoot() / "semantics/pattern-expression-name-conflict.txt";
    FileNode file = Parse(path);

    CHECK_THROWS_WITH_AS(Analyze(file),
                         "Error on line 5: 'name' already defined on line 2",
                         PlexiException);
}

TEST_CASE("Semantics: Expression with name that duplicates a pattern")
{
    std::filesystem::path path =
        GetTestRoot() / "semantics/expression-pattern-name-conflict.txt";
    FileNode file = Parse(path);

    CHECK_THROWS_WITH_AS(Analyze(file),
                         "Error on line 5: 'name' already defined on line 2",
                         PlexiException);
}

TEST_CASE("Semantics: Rule that uses an undefined name")
{
    std::filesystem::path path =
        GetTestRoot() / "semantics/rule-name-undefined.txt";
    FileNode file = Parse(path);

    CHECK_THROWS_WITH_AS(Analyze(file),
                         "Error on line 2: Undefined name 'name'",
                         PlexiException);
}

TEST_CASE("Semantics: Pattern that uses an undefined name")
{
    std::filesystem::path path =
        GetTestRoot() / "semantics/pattern-name-undefined.txt";
    FileNode file = Parse(path);

    CHECK_THROWS_WITH_AS(Analyze(file), "Error on line 3: Undefined name 'abc'",
                         PlexiException);
}

TEST_CASE("Semantics: Pattern that uses an undefined name far in")
{
    std::filesystem::path path =
        GetTestRoot() / "semantics/pattern-name-undefined-far-in.txt";
    FileNode file = Parse(path);

    CHECK_THROWS_WITH_AS(Analyze(file),
                         "Error on line 9: Undefined name 'missing'",
                         PlexiException);
}

TEST_CASE("Semantics: Rule that uses rewind")
{
    std::filesystem::path path =
        GetTestRoot() / "semantics/rule-using-rewind.txt";
    FileNode file = Parse(path);

    CHECK_THROWS_WITH_AS(Analyze(file),
                         "Error on line 6: 'rewind' action not yet supported",
                         PlexiException);
}

TEST_CASE("Semantics: Reject pattern statements")
{
    std::filesystem::path path =
        GetTestRoot() / "semantics/reject-pattern-statements.txt";
    FileNode file = Parse(path);

    CHECK_THROWS_WITH_AS(
        Analyze(file), "Error on line 5: 'pattern' statement not yet supported",
        PlexiException);
}

TEST_CASE("Semantics: Reject duplicate produce-nothing")
{
    std::filesystem::path path =
        GetTestRoot() / "semantics/rule-duplicate-produce-nothing.txt";
    FileNode file = Parse(path);

    CHECK_THROWS_WITH_AS(
        Analyze(file),
        "Error on line 7: `produce-nothing` already used in rule on line 6",
        PlexiException);
}

TEST_CASE("Semantics: Reject duplicate produce")
{
    std::filesystem::path path =
        GetTestRoot() / "semantics/rule-duplicate-produce.txt";
    FileNode file = Parse(path);

    CHECK_THROWS_WITH_AS(
        Analyze(file),
        "Error on line 7: `produce` already used in rule on line 6",
        PlexiException);
}

TEST_CASE("Semantics: Reject duplicate rewind")
{
    std::filesystem::path path =
        GetTestRoot() / "semantics/rule-duplicate-rewind.txt";
    FileNode file = Parse(path);

    CHECK_THROWS_WITH_AS(
        Analyze(file),
        "Error on line 7: `rewind` already used in rule on line 6",
        PlexiException);
}

TEST_CASE("Semantics: Reject duplicate transition")
{
    std::filesystem::path path =
        GetTestRoot() / "semantics/rule-duplicate-transition.txt";
    FileNode file = Parse(path);

    CHECK_THROWS_WITH_AS(
        Analyze(file),
        "Error on line 7: `transition` already used in rule on line 6",
        PlexiException);
}

TEST_CASE("Semantics: Reject duplicate increment")
{
    std::filesystem::path path =
        GetTestRoot() / "semantics/rule-duplicate-increment.txt";
    FileNode file = Parse(path);

    CHECK_THROWS_WITH_AS(
        Analyze(file),
        "Error on line 7: `++line` already used in rule on line 6",
        PlexiException);
}

TEST_CASE("Semantics: Reject mismatched duplicate increment")
{
    std::filesystem::path path =
        GetTestRoot() / "semantics/rule-duplicate-mismatched-increment.txt";
    FileNode file = Parse(path);

    CHECK_THROWS_WITH_AS(
        Analyze(file),
        "Error on line 7: `line++` already used in rule on line 6",
        PlexiException);
}

TEST_CASE("Semantics: Reject duplicate decrement")
{
    std::filesystem::path path =
        GetTestRoot() / "semantics/rule-duplicate-decrement.txt";
    FileNode file = Parse(path);

    CHECK_THROWS_WITH_AS(
        Analyze(file),
        "Error on line 7: `--line` already used in rule on line 6",
        PlexiException);
}

TEST_CASE("Semantics: Reject mismatched duplicate decrement")
{
    std::filesystem::path path =
        GetTestRoot() / "semantics/rule-duplicate-mismatched-decrement.txt";
    FileNode file = Parse(path);

    CHECK_THROWS_WITH_AS(
        Analyze(file),
        "Error on line 7: `line--` already used in rule on line 6",
        PlexiException);
}

TEST_CASE("Semantics: Reject rule with transition to self (initial state)")
{
    std::filesystem::path path =
        GetTestRoot() / "semantics/rule-transition-to-self-initial.txt";
    FileNode file = Parse(path);

    CHECK_THROWS_WITH_AS(
        Analyze(file),
        "Error on line 6: Rule transitions to its own state.",
        PlexiException);
}

TEST_CASE("Semantics: Allow rule with transition to undefined state __jail__")
{
    std::filesystem::path path =
        GetTestRoot() / "semantics/rule-transition-to-jail.txt";
    FileNode file = Parse(path);

    CHECK_NOTHROW(Analyze(file));
}

TEST_CASE("Semantics: Allow rule with transition to undefined state __initial__")
{
    std::filesystem::path path =
        GetTestRoot() / "semantics/rule-transition-to-initial.txt";
    FileNode file = Parse(path);

    CHECK_NOTHROW(Analyze(file));
}

TEST_CASE("Semantics: Reject rule with transition to self (other state)")
{
    std::filesystem::path path =
        GetTestRoot() / "semantics/rule-transition-to-self-other.txt";
    FileNode file = Parse(path);

    CHECK_THROWS_WITH_AS(Analyze(file),
                         "Error on line 10: Rule transitions to its own state.",
                         PlexiException);
}

TEST_CASE("Semantics: Reject rule with unreachable state")
{
    std::filesystem::path path =
        GetTestRoot() / "semantics/rule-unreachable-state.txt";
    FileNode file = Parse(path);

    CHECK_THROWS_WITH_AS(
        Analyze(file),
        "Error on line 9: No rule transitions to state `unreachable`",
        PlexiException);
}

TEST_CASE("Semantics: Reject rule with transition to nowhere")
{
    std::filesystem::path path =
        GetTestRoot() / "semantics/rule-transition-to-nowhere.txt";
    FileNode file = Parse(path);

    CHECK_THROWS_WITH_AS(
        Analyze(file),
        "Error on line 6: No rules for state `nowhere`",
        PlexiException);
}
