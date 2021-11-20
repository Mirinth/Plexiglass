#include <filesystem>

#include "doctest.h"

#include <analyzer/analyzer.hpp>
#include <parser/parser.hpp>
#include <parser/tree.hpp>
#include <template/template.hpp>

#include "test_files.hpp"

TEST_CASE("Template: Test template")
{
    std::filesystem::path testDir = GetTestRoot() / "template/";

    std::string testName = "template_test";
    std::filesystem::path header = testDir / "template_test-out.hpp";
    std::filesystem::path code = testDir / "template_test-out.cpp";
    std::filesystem::path source = testDir / "template_test-in.txt";

    FileNode file = Parse(source);
    Analyze(file);
    Template(file, testName, header, code, false);

    std::string base = ReadTestFile("template/template_test-base.hpp");
    std::string out = ReadTestFile("template/template_test-out.hpp");
    CHECK(base == out);

    base = ReadTestFile("template/template_test-base.cpp");
    out = ReadTestFile("template/template_test-out.cpp");
    CHECK(base == out);
}

TEST_CASE("Template: Test template with debug driver")
{
    std::filesystem::path testDir = GetTestRoot() / "template/";

    std::string testName = "template_test_debug";
    std::filesystem::path header = testDir / "template_test_debug-out.hpp";
    std::filesystem::path code = testDir / "template_test_debug-out.cpp";
    std::filesystem::path source = testDir / "template_test_debug-in.txt";

    FileNode file = Parse(source);
    Analyze(file);
    Template(file, testName, header, code, true);

    std::string base = ReadTestFile("template/template_test_debug-base.hpp");
    std::string out = ReadTestFile("template/template_test_debug-out.hpp");
    CHECK(base == out);

    base = ReadTestFile("template/template_test_debug-base.cpp");
    out = ReadTestFile("template/template_test_debug-out.cpp");
    CHECK(base == out);
}
