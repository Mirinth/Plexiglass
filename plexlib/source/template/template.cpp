#include <template/template.hpp>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>

#include <template-holder.hpp>
#include <utils.hpp>

std::string MakeUnique(const std::set<std::string>& names,
                       const std::string& name)
{
    std::string uniqueName = name;
    for (size_t unique = 0; unique < std::numeric_limits<size_t>::max()
                            && names.count(uniqueName) != 0;
         unique++)
    {
        uniqueName = name + "_" + std::to_string(unique);
    }
    return uniqueName;
}

void ReplaceEofName(std::string& content, const std::string& name)
{
    Replace(content, "$EOF_TOKEN", name);
}

void ReplaceErrorName(std::string& content, const std::string& name)
{
    Replace(content, "$INVALID_TOKEN", name);
}

void ReplaceName(std::string& content, const std::string& name)
{
    Replace(content, "$LEXER_NAME", name);
}

std::tuple<std::string, std::string> ReplaceTokens(std::string& content,
                                                   FileNode file)
{
    std::set<std::string> tokenNames;
    file->GetTokenNames(tokenNames);

    std::string errorName = MakeUnique(tokenNames, "PLEXIGLASS_NO_MATCH_TOKEN");
    tokenNames.insert(errorName);

    std::string eofName = MakeUnique(tokenNames, "PLEXIGLASS_EOF");
    tokenNames.insert(eofName);

    std::stringstream names;
    for (auto& tokenName : tokenNames)
    {
        names << "\n    " << tokenName << ",";
    }

    std::string namesStr = names.str();
    namesStr.erase(0, 5); // Erase the leading "\n    "

    Replace(content, "$TOKEN_NAMES", namesStr);

    return std::make_tuple(eofName, errorName);
}

void ReplaceRules(std::string& content, FileNode file, std::string errorName)
{
    std::string ruleString = file->GetRuleString(errorName);
    Replace(content, "$LEXER_RULES", ruleString);
}

void ReplaceToString(std::string& content,
                     FileNode file,
                     std::string eofName,
                     std::string errorName)
{
    std::set<std::string> tokenNames;
    file->GetTokenNames(tokenNames);
    tokenNames.insert(eofName);
    tokenNames.insert(errorName);

    std::stringstream out;
    for (const std::string& name : tokenNames)
    {
        out << "case " << name << ":\n\t\t\treturn \"" << name << "\";\n\t\t";
    }

    out << "default:\n\t\t\tthrow std::exception(\"Unknown token\");";

    Replace(content, "$TOKEN_TO_STRING", out.str());
}

void SaveFile(const std::string& content, const std::string& path)
{
    std::ofstream out(path);
    out << content;
}

std::tuple<std::string, std::string> TemplateHeader(FileNode file,
                                                    std::string header,
                                                    std::string name)
{
    std::string content = header_template;

    ReplaceName(content, name);
    auto names = ReplaceTokens(content, file);

    SaveFile(content, header);

    return names;
}

void TemplateBody(FileNode file,
                  std::string eofName,
                  std::string errorName,
                  std::string code,
                  std::string name)
{
    std::string content = code_template;

    ReplaceErrorName(content, errorName);
    ReplaceEofName(content, eofName);
    ReplaceName(content, name);
    ReplaceRules(content, file, eofName);
    ReplaceToString(content, file, eofName, errorName);
    SaveFile(content, code);
}

void Template(FileNode file,
              std::string name,
              std::string header,
              std::string code)
{
    std::filesystem::remove(header);
    std::filesystem::remove(code);
    auto [eofName, errorName] = TemplateHeader(file, header, name);
    TemplateBody(file, eofName, errorName, code, name);
}
