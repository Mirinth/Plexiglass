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
