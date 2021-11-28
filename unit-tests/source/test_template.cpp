#include <filesystem>

#include "doctest.h"

#include <analyzer/analyzer.hpp>
#include <parser/parser.hpp>
#include <parser/tree.hpp>
#include <template/template.hpp>

#include "test_files.hpp"

void TemplateTest(std::string name, bool debug)
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

TEST_CASE("Template: Test template")
{
    TemplateTest("simple", false);
}

TEST_CASE("Template: Test template with debug driver")
{
    TemplateTest("debug", true);
}
