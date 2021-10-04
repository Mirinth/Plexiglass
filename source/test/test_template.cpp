#include <filesystem>

#include "doctest.h"

#include <analyzer/analyzer.hpp>
#include <parser/parser.hpp>
#include <parser/tree.hpp>
#include <template/template.hpp>

#include "read_test_file.hpp"

TEST_CASE("Template: Test template")
{
    std::string testDir = "../../../tests/template/";

    std::string testName = "L101";
    std::string header = testDir + "L101-out.hpp";
    std::string code = testDir + "L101-out.cpp";
    std::string source = "template/L101-in.txt";

    std::string data = ReadTestFile(source);
    FileNode file = Parse(data);
    Analyze(file);
    Template(file, testName, header, code);

    std::string base = ReadTestFile("template/L101-base.hpp");
    std::string out = ReadTestFile("template/L101-out.hpp");
    CHECK(base == out);

    base = ReadTestFile("template/L101-base.cpp");
    out = ReadTestFile("template/L101-out.cpp");
    CHECK(base == out);
}
