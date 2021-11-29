#include <filesystem>

#include "doctest.h"

#include <analyzer/analyzer.hpp>
#include <parser/parser.hpp>
#include <parser/tree.hpp>
#include <templater/templater.hpp>

#include "test_files.hpp"

void TemplaterTest(std::string name, bool debug)
{
    std::filesystem::path testDir = GetTestRoot() / "template/";

    std::string testName = name;
    std::filesystem::path header = testDir / (name + "-out.hpp");
    std::filesystem::path code = testDir / (name + "-out.cpp");
    std::filesystem::path source = testDir / (name + "-in.txt");

    FileNode file = Parse(source);
    Analyze(file);
    Template(file, testName, header, code, debug);

    std::string base = ReadTestFile("template/" + name + "-base.hpp");
    std::string out = ReadTestFile("template/" + name + "-out.hpp");
    CHECK(base == out);

    base = ReadTestFile("template/" + name + "-base.cpp");
    out = ReadTestFile("template/" + name + "-out.cpp");
    CHECK(base == out);
}

TEST_CASE("Templater: Test template")
{
    TemplaterTest("simple", false);
}

TEST_CASE("Templater: Test template with debug driver")
{
    TemplaterTest("debug", true);
}

TEST_CASE("Templater: Test template with all features")
{
    TemplaterTest("full", true);
}
