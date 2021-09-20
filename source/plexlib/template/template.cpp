#include <template/template.hpp>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>

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

void ReplaceName(std::string& content, const std::string& name)
{
    Replace(content, "$LEXER_NAME", name);
}

void ReplaceTokens(std::string& content, FileNode file)
{
    std::set<std::string> tokenNames;
    file->GetTokenNames(tokenNames);
    std::stringstream names;
    for (auto& tokenName : tokenNames)
    {
        names << "\n\t" << tokenName << ",";
    }
    Replace(content, "$TOKEN_NAMES", names.str());
}

void SaveFile(const std::string& content,
              const std::string& dir,
              const std::string& name,
              const std::string& ext)
{
    std::filesystem::path path = std::filesystem::path(dir) / name;
    path.replace_extension(ext);
    std::ofstream out(path);
    out << content;
}

void TemplateHeader(FileNode file, std::string dir, std::string name)
{
    std::string content = ReadFile("template.hpp");
    
    ReplaceName(content, name);
    ReplaceTokens(content, file);

    SaveFile(content, dir, name, ".hpp");
}

void TemplateBody(FileNode file, std::string dir, std::string name)
{
    std::string content = ReadFile("template.cpp");

    ReplaceName(content, name);

    SaveFile(content, dir, name, ".cpp");
}

void Template(FileNode file, std::string dir, std::string name)
{
    std::filesystem::create_directories(dir);
    TemplateHeader(file, dir, name);
    TemplateBody(file, dir, name);
}
