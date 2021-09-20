#include <template/template.hpp>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>

std::string ReadFile(std::string path);

// https://stackoverflow.com/a/3418285
void Replace(std::string& subject,
             const std::string& find,
             const std::string& replace)
{
    if (find.empty())
    {
        return;
    }

    size_t start_pos = 0;
    while ((start_pos = subject.find(find, start_pos)) != std::string::npos)
    {
        subject.replace(start_pos, find.length(), replace);
        start_pos += replace.length(); // In case 'replace' contains 'find',
                                       // like replacing 'x' with 'yx'
    }
}


void TemplateHeader(FileNode /*file*/, std::string dir, std::string name)
{
    std::filesystem::path headerPath = std::filesystem::path(dir) / name;
    headerPath.replace_extension(".hpp");

    std::string headerContent = ReadFile("template.hpp");

    Replace(headerContent, "$LEXER_NAME", name);

    std::ofstream out(headerPath);
    out << headerContent;
}

void Template(FileNode file, std::string dir, std::string name)
{
    std::filesystem::create_directories(dir);
    TemplateHeader(file, dir, name);
}