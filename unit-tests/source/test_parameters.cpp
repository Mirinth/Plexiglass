#include <filesystem>
#include <sstream>

#include "doctest.h"

#include <plexlib.hpp>

TEST_CASE("Parameters: No parameters")
{
    std::stringstream out, err, base;
    std::vector<std::string> params = {};

    PrintUsage(base);
    int result = PlexMain(params, out, err);

    CHECK(bad_usage == result);
    CHECK(base.str() == out.str());
    CHECK("" == err.str());
}

TEST_CASE("Parameters: -h")
{
    std::stringstream out, err, base;
    std::vector<std::string> params = { "-h" };

    PrintUsage(base);
    int result = PlexMain(params, out, err);

    CHECK(help_invoked == result);
    CHECK(base.str() == out.str());
    CHECK("" == err.str());
}

TEST_CASE("Parameters: Extra parameters")
{
    std::stringstream out, err, base;
    std::vector<std::string> params = { "-h", "-h" };

    PrintUsage(base);
    int result = PlexMain(params, out, err);

    CHECK(bad_usage == result);
    CHECK(base.str() == out.str());
    CHECK("" == err.str());
}

TEST_CASE("Parameters: Nonexistent file")
{
    std::stringstream out, err;
    std::filesystem::path inputFile(
        "z:/path/to/file/that/doesn't/exist/nonexistent.txt");
    inputFile.make_preferred();
    std::vector<std::string> params = { inputFile.string() };

    int result = PlexMain(params, out, err);

    CHECK(unreadable_file == result);
    CHECK("" == out.str());
    CHECK("Unable to read file " + inputFile.string() + "\n" == err.str());
}

TEST_CASE("Parameters: File with bad identifier")
{
    std::stringstream out, err;
    std::string base =
        "File generates lexer named `c++` which is not a valid C++ identifier. "
        "Rename the file to be a valid C++ identifier.\n";
    std::filesystem::path inputFile("z:/c++.txt");
    inputFile.make_preferred();
    std::vector<std::string> params = { inputFile.string() };

    int result = PlexMain(params, out, err);

    CHECK(bad_lexer_name == result);
    CHECK("" == out.str());
    CHECK(base == err.str());
}
