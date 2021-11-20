#pragma once

#include <filesystem>
#include <string>

constexpr char* test_root = "${PLEXTEST_UNIT_TEST_DIR}/";
std::filesystem::path GetTestRoot();
std::string ReadTestFile(std::string name);
