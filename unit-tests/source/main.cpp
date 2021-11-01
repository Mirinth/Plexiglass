#include <filesystem>
#include <string>

#include <utils.hpp>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

std::string GetTestRoot()
{
    auto root = std::filesystem::absolute("./");
    return root.string();
}

std::string ReadTestFile(std::string name)
{
    std::string path = GetTestRoot() + name;
    return ReadFile(path);
}
