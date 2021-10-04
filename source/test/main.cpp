#include <string>

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
