#pragma once

#include <filesystem>
#include <string>

#include <parser/tree.hpp>

void Template(FileNode file,
              std::string name,
              std::filesystem::path header,
              std::filesystem::path code,
              bool debug);
