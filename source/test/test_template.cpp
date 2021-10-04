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

    std::string testName = "template_test";
    std::string header = testDir + "template_test-out.hpp";
    std::string code = testDir + "template_test-out.cpp";
    std::string source = "template/template_test-in.txt";

    std::string data = ReadTestFile(source);
    FileNode file = Parse(data);
    Analyze(file);
    Template(file, testName, header, code);

    std::string base = ReadTestFile("template/template_test-base.hpp");
    std::string out = ReadTestFile("template/template_test-out.hpp");
    CHECK(base == out);

    base = ReadTestFile("template/template_test-base.cpp");
    out = ReadTestFile("template/template_test-out.cpp");
    CHECK(base == out);
}
