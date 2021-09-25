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

void ReplaceErrorName(std::string& content, const std::string& name)
{
    Replace(content, "$INVALID_TOKEN", name);
}

void ReplaceName(std::string& content, const std::string& name)
{
    Replace(content, "$LEXER_NAME", name);
}

std::string ReplaceTokens(std::string& content, FileNode file)
{
    std::set<std::string> tokenNames;
    file->GetTokenNames(tokenNames);
    
    std::string errorName = "PLEXIGLASS_NO_MATCH_TOKEN";
    for (size_t unique = 0; unique < std::numeric_limits<size_t>::max() && tokenNames.count(errorName) != 0; unique++)
    {
        errorName = "PLEXIGLASS_NO_MATCH_TOKEN_" + std::to_string(unique);
    }
    tokenNames.insert(errorName);

    std::stringstream names;
    for (auto& tokenName : tokenNames)
    {
        names << "\n\t" << tokenName << ",";
    }

    Replace(content, "$TOKEN_NAMES", names.str());
    
    return errorName;
}

void ReplaceRules(std::string& content, FileNode file, std::string errorName)
{
    std::string ruleString = file->GetRuleString(errorName);
    Replace(content, "$LEXER_RULES", ruleString);
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

std::string TemplateHeader(FileNode file, std::string dir, std::string name)
{
    std::string content = ReadFile("template.hpp");
    
    ReplaceName(content, name);
    std::string errorName = ReplaceTokens(content, file);

    SaveFile(content, dir, name, ".hpp");

    return errorName;
}

void TemplateBody(FileNode file,
                  std::string errorName,
                  std::string dir,
                  std::string name)
{
    std::string content = ReadFile("template.cpp");

    ReplaceErrorName(content, errorName);
    ReplaceName(content, name);
    ReplaceRules(content, file, errorName);
    SaveFile(content, dir, name, ".cpp");
}

void Template(FileNode file, std::string dir, std::string name)
{
    std::filesystem::create_directories(dir);
    std::string errorName = TemplateHeader(file, dir, name);
    TemplateBody(file, errorName, dir, name);
}
