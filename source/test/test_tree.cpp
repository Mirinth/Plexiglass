#include <sstream>

#include "doctest.h"

#include <parser/parser.hpp>
#include <parser/tree.hpp>

#include "read_test_file.hpp"

std::string BuildTree(std::string path)
{
    std::string data = ReadTestFile(path);
    FileNode file = Parse(data);
    std::stringstream out;
    out << file;
    return out.str();
}

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
