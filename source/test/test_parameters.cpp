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
