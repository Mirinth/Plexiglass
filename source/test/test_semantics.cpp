#include <sstream>

#include "doctest.h"

#include <analyzer/analyzer.hpp>
#include <error.hpp>
#include <parser/parser.hpp>
#include <parser/tree.hpp>

#include "read_test_file.hpp"

TEST_CASE("Semantics: Expression with duplicate name")
{
    std::string data = ReadTestFile("semantics/expression-duplicate-name.txt");
    FileNode file = Parse(data);

    CHECK_THROWS_WITH_AS(Analyze(file),
                         "Error on line 5: 'name' already defined on line 2",
                         PlexiException);
}