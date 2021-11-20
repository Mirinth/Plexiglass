#include <filesystem>
#include <string>

#include <test_files.hpp>
#include <utils.hpp>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

/// <summary>
/// Gets an absolute path to the folder where the test files are located.
/// Assumes the current directory is where test files are located.
/// </summary>
/// <returns>Absolute path to test files folder.</returns>
std::filesystem::path GetTestRoot()
{
    return test_root;
}

/// <summary>
/// Read the contents of a test file in as a string.
/// </summary>
/// <param name="name">
/// Path to the test file, relative to the current directory.
/// </param>
/// <returns>The contents of the test file.</returns>
std::string ReadTestFile(std::string name)
{
    std::filesystem::path path = GetTestRoot() / name;
    return ReadFile(path);
}
