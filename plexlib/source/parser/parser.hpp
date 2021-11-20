#pragma once

#include <filesystem>

#include <parser/tree.hpp>

FileNode Parse(std::filesystem::path path);
