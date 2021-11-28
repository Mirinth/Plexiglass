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

    std::string testName = "simple";
    std::filesystem::path header = testDir / "simple-out.hpp";
    std::filesystem::path code = testDir / "simple-out.cpp";
    std::filesystem::path source = testDir / "simple-in.txt";

    FileNode file = Parse(source);
    Analyze(file);
    Template(file, testName, header, code, false);

    std::string base = ReadTestFile("template/simple-base.hpp");
    std::string out = ReadTestFile("template/simple-out.hpp");
    CHECK(base == out);

    base = ReadTestFile("template/simple-base.cpp");
    out = ReadTestFile("template/simple-out.cpp");
    CHECK(base == out);
}

TEST_CASE("Template: Test template with debug driver")
{
    std::filesystem::path testDir = GetTestRoot() / "template/";

    std::string testName = "debug";
    std::filesystem::path header = testDir / "debug-out.hpp";
    std::filesystem::path code = testDir / "debug-out.cpp";
    std::filesystem::path source = testDir / "debug-in.txt";

    FileNode file = Parse(source);
    Analyze(file);
    Template(file, testName, header, code, true);

    std::string base = ReadTestFile("template/debug-base.hpp");
    std::string out = ReadTestFile("template/debug-out.hpp");
    CHECK(base == out);

    base = ReadTestFile("template/debug-base.cpp");
    out = ReadTestFile("template/debug-out.cpp");
    CHECK(base == out);
}
