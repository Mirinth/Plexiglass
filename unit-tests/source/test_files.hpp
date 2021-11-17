#pragma once

#include <string>

constexpr char* test_root = "${PLEXTEST_UNIT_TEST_DIR}/";
std::string GetTestRoot();
std::string ReadTestFile(std::string name);
