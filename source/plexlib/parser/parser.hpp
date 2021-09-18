#pragma once

#include <std/string_view>

#include <parser/tree.hpp>

FileNode Parse(std::string_view data);
