#include <sstream>

#include "doctest.h"

#include <parser/parser.hpp>
#include <parser/tree.hpp>

#include "test_files.hpp"

/// <summary>
/// Build a parse tree and serialize it to a string.
/// </summary>
/// <param name="path">Path to the file to parse.</param>
/// <returns>The parse tree, serialized to a string.</returns>
std::string BuildTree(std::string path)
{
    FileNode file = Parse(path);
    std::stringstream out;
    out << file;
    return out.str();
}

// ============================================================================
// Expressions
// ============================================================================

TEST_CASE("Tree: Test a single expression")
{
    std::string base = ReadTestFile("tree/expression-single-base.txt");
    std::string out = BuildTree("tree/expression-single.txt");
    CHECK(base == out);
}

TEST_CASE("Tree: Test two expressions")
{
    std::string base = ReadTestFile("tree/expression-two-base.txt");
    std::string out = BuildTree("tree/expression-two.txt");
    CHECK(base == out);
}

TEST_CASE("Tree: Test many expressions")
{
    std::string base = ReadTestFile("tree/expression-many-base.txt");
    std::string out = BuildTree("tree/expression-many.txt");
    CHECK(base == out);
}

// ============================================================================
// Patterns
// ============================================================================

TEST_CASE("Tree: Pattern with one alternative and one symbol")
{
    std::string base =
        ReadTestFile("tree/pattern-one-alternative-one-symbol-base.txt");
    std::string out = BuildTree("tree/pattern-one-alternative-one-symbol.txt");
    CHECK(base == out);
}

TEST_CASE("Tree: Pattern with one alternative and two symbols")
{
    std::string base =
        ReadTestFile("tree/pattern-one-alternative-two-symbols-base.txt");
    std::string out = BuildTree("tree/pattern-one-alternative-two-symbols.txt");
    CHECK(base == out);
}

TEST_CASE("Tree: Pattern with one alternative and many symbols")
{
    std::string base =
        ReadTestFile("tree/pattern-one-alternative-many-symbols-base.txt");
    std::string out =
        BuildTree("tree/pattern-one-alternative-many-symbols.txt");
    CHECK(base == out);
}

TEST_CASE("Tree: Pattern with two alternatives with one symbol each")
{
    std::string base =
        ReadTestFile("tree/pattern-two-alternatives-one-symbol-base.txt");
    std::string out = BuildTree("tree/pattern-two-alternatives-one-symbol.txt");
    CHECK(base == out);
}

TEST_CASE("Tree: Pattern with many alternatives with many symbols each")
{
    std::string base =
        ReadTestFile("tree/pattern-many-alternatives-many-symbols-base.txt");
    std::string out =
        BuildTree("tree/pattern-many-alternatives-many-symbols.txt");
    CHECK(base == out);
}

TEST_CASE(
    "Tree: Pattern with multiple alternatives each with multiple alternatives")
{
    std::string base =
        ReadTestFile("tree/multiple-patterns-multiple-alternatives-base.txt");
    std::string out =
        BuildTree("tree/multiple-patterns-multiple-alternatives.txt");
    CHECK(base == out);
}

// ============================================================================
// Rules
// ============================================================================

TEST_CASE("Tree: Single rule")
{
    std::string base = ReadTestFile("tree/rule-single-base.txt");
    std::string out = BuildTree("tree/rule-single.txt");
    CHECK(base == out);
}

TEST_CASE("Tree: Two rules")
{
    std::string base = ReadTestFile("tree/rule-two-base.txt");
    std::string out = BuildTree("tree/rule-two.txt");
    CHECK(base == out);
}

TEST_CASE("Tree: Many rules")
{
    std::string base = ReadTestFile("tree/rule-many-base.txt");
    std::string out = BuildTree("tree/rule-many.txt");
    CHECK(base == out);
}

TEST_CASE("Tree: Rule with many actions")
{
    std::string base = ReadTestFile("tree/rule-many-actions-base.txt");
    std::string out = BuildTree("tree/rule-many-actions.txt");
    CHECK(base == out);
}
