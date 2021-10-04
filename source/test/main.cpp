#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include <analyzer/analyzer.hpp>
#include <error.hpp>
#include <lexer/lexer.hpp>
#include <parser/parser.hpp>
#include <template/template.hpp>
#include <utils.hpp>

#pragma warning(push)
#pragma warning(disable : 5245)
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#pragma warning(pop)

std::string ReadTestFile(std::string name)
{
    std::string path = "../../../tests/" + name;
    return ReadFile(path);
}

typedef std::function<bool(std::string)> Tester;

bool IsInputFile(std::filesystem::path file)
{
    if (file.extension() != ".txt")
    {
        return false;
    }

    std::string stem = file.stem().string();
    std::string suffix = "-in";

    if (stem.size() < suffix.size())
    {
        return false;
    }

    return std::equal(suffix.rbegin(), suffix.rend(), stem.rbegin());
}

std::vector<std::string> GetTestStems(std::string directory)
{
    std::string testDir = "../../../tests/" + directory;
    std::vector<std::string> stems;
    std::filesystem::path base = std::filesystem::absolute(testDir);

    for (auto& file : std::filesystem::directory_iterator(base))
    {
        if (!IsInputFile(file))
        {
            continue;
        }

        std::string stem = file.path().stem().string();
        size_t location = stem.rfind('-');
        stem.erase(location);
        stems.push_back((base / stem).string());
    }

    std::sort(stems.begin(), stems.end());

    return stems;
}

bool CompareOutput(std::string basePath, std::string outPath)
{
    std::ifstream base(basePath);
    std::ifstream out(outPath);

    while (base.good() && out.good())
    {
        if (base.get() != out.get())
        {
            return false;
        }
    }

    if (base.eof() && out.eof())
    {
        return true;
    }

    return false;
}

bool TestGroup(std::string name, Tester test)
{
    auto stems = GetTestStems(name);

    for (auto& stem : stems)
    {
        std::cout << stem + "-in.txt : ";

        bool pass = test(stem);

        if (pass)
        {
            std::cout << "PASS\n";
        }
        else
        {
            std::cout << "FAIL\n";
            return false;
        }
    }

    return true;
}

//int main()
//{
//    std::vector<std::tuple<std::string, Tester>> map = {
//        { "semantics", RunAnalyzerTest }, { "template", RunTemplateTest },
//    };
//
//    for (auto& [name, tester] : map)
//    {
//        bool success = TestGroup(name, tester);
//        if (!success)
//        {
//            return 1;
//        }
//    }
//
//    std::cout << "\n\nNo errors\n\n";
//
//    return 0;
//}
