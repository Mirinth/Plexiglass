#pragma once

#include <string_view>

#include <parser/tree.hpp>

FileNode Parse(std::string_view data);
