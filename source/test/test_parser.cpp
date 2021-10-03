#include <string>

#include <error.hpp>
#include <parser/parser.hpp>

#include "doctest.h"

#include "read_test_file.hpp"

TEST_CASE("Test the parser")
{
    std::string data = ReadTestFile("parser/empty-file-in.txt");
    CHECK_THROWS_WITH_AS(Parse(data), "Error on line 1: Expected keyword found eof", PlexiException);
}
