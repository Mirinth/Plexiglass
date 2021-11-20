#pragma once

#include <filesystem>
#include <string>

std::string ReadFile(std::filesystem::path path);
void Replace(std::string& subject,
             const std::string& find,
             const std::string& replace);