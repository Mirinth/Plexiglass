#include <template/template.hpp>

#include <filesystem>
#include <fstream>
#include <iostream>

std::string ReadFile(std::string path);

void TemplateHeader(FileNode /*file*/, std::string dir, std::string name)
{
    std::filesystem::path headerPath = std::filesystem::path(dir) / name;
    headerPath.replace_extension(".hpp");

    std::string headerContent = ReadFile("template.hpp");

    std::ofstream out(headerPath);
    out << headerContent;
}

void Template(FileNode file, std::string dir, std::string name)
{
    std::filesystem::create_directories(dir);
    TemplateHeader(file, dir, name);
}