#include <sstream>

#include "doctest.h"

#include <plexlib.hpp>

TEST_CASE("Parameters: No parameters")
{
    std::stringstream out, err, base;
    std::vector<std::string> params = {};

    PrintUsage(base);
    PlexMain(params, out, err);

    CHECK(out.str() == base.str());
    CHECK(err.str() == "");
}

TEST_CASE("Parameters: -h")
{
    std::stringstream out, err, base;
    std::vector<std::string> params = { "-h" };

    PrintUsage(base);
    PlexMain(params, out, err);

    CHECK(out.str() == base.str());
    CHECK(err.str() == "");
}
