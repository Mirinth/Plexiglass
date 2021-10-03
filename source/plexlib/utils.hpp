#pragma once

#include <string>

std::string ReadFile(std::string path);
void Replace(std::string& subject,
             const std::string& find,
             const std::string& replace);