#include <template/template.hpp>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>

#include <template-holder.hpp>
#include <utils.hpp>

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
    for (size_t unique = 0; unique < std::numeric_limits<size_t>::max()
                            && tokenNames.count(errorName) != 0;
         unique++)
    {
        errorName = "PLEXIGLASS_NO_MATCH_TOKEN_" + std::to_string(unique);
    }
    tokenNames.insert(errorName);

    std::stringstream names;
    for (auto& tokenName : tokenNames)
    {
        names << "\n\t" << tokenName << ",";
    }

    std::string namesStr = names.str();
    namesStr.erase(0, 2); // Erase the leading "\n\t"

    Replace(content, "$TOKEN_NAMES", namesStr);

    return errorName;
}

void ReplaceRules(std::string& content, FileNode file, std::string errorName)
{
    std::string ruleString = file->GetRuleString(errorName);
    Replace(content, "$LEXER_RULES", ruleString);
}

void ReplaceToString(std::string& content, FileNode /*file*/)
{
    Replace(content, "$TOKEN_TO_STRING", "Implementation;");
}

void SaveFile(const std::string& content, const std::string& path)
{
    std::ofstream out(path);
    out << content;
}

std::string TemplateHeader(FileNode file, std::string header, std::string name)
{
    std::string content = header_template;

    ReplaceName(content, name);
    std::string errorName = ReplaceTokens(content, file);

    SaveFile(content, header);

    return errorName;
}

void TemplateBody(FileNode file,
                  std::string errorName,
                  std::string code,
                  std::string name)
{
    std::string content = code_template;

    ReplaceErrorName(content, errorName);
    ReplaceName(content, name);
    ReplaceRules(content, file, errorName);
    ReplaceToString(content, file);
    SaveFile(content, code);
}

void Template(FileNode file,
              std::string name,
              std::string header,
              std::string code)
{
    std::filesystem::remove(header);
    std::filesystem::remove(code);
    std::string errorName = TemplateHeader(file, header, name);
    TemplateBody(file, errorName, code, name);
}
