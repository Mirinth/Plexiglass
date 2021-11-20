#include <template/template.hpp>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>

#include <template-holder.hpp>
#include <utils.hpp>

/// <summary>
/// Get names defined by a lexer.
/// </summary>
/// <param name="node">The lexer.</param>
/// <param name="names">Populated with defined names.</param>
void GetTokenNames(FileNode node, std::set<std::string>& names)
{
    names.clear();

    for (const auto& rule : node->rules)
    {
        for (const auto& action : rule->actions)
        {
            if (action->name == "produce")
            {
                names.insert(action->identifier);
            }
        }
    }
}

/// <summary>
/// Modify a Rule's members to match an action.
/// </summary>
/// <param name="node">The action.</param>
/// <param name="rule">The rule.</param>
void GetRule(ActionNode node, Rule& rule)
{
    if (node->name == "produce-nothing")
    {
        rule.Produces = false;
    }
    else if (node->name == "produce")
    {
        rule.Produces = true;
        rule.Token = node->identifier;
    }
    else if (node->name == "++line" || node->name == "line++")
    {
        rule.Increment = 1;
    }
    else if (node->name == "--line" || node->name == "line--")
    {
        rule.Increment = -1;
    }
    else
    {
        throw std::exception("Illegal action name");
    }
}

/// <summary>
/// Modify a Rule's members to match a rule.
/// </summary>
/// <param name="node"></param>
/// <returns></returns>
Rule GetRule(RuleNode node)
{
    Rule rule = { false, "", 0, node->name };

    for (const auto& action : node->actions)
    {
        GetRule(action, rule);
    }

    return rule;
}

/// <summary>
/// Get the string to be substituted into the template's rule location.
/// </summary>
/// <param name="node">The lexer.</param>
/// <param name="illegalTokenName">
/// Name to use for the token generated when no other token is.
/// </param>
/// <returns>The rule string.</returns>
std::string GetRuleString(FileNode node, std::string illegalTokenName)
{
    std::vector<Rule> producedRules;

    for (auto& rule : node->rules)
    {
        Rule producedRule = GetRule(rule);

        for (auto& expression : node->expressions)
        {
            if (expression->name == producedRule.Pattern)
            {
                producedRule.Pattern = expression->expression;

                // Needs to come first so \ inserted by next one aren't escaped
                Replace(producedRule.Pattern, "\\",
                        "\\\\"); // Escape backslaches
                Replace(producedRule.Pattern, "\"",
                        "\\\""); // Escape double quotes

                producedRule.Pattern = "\"" + producedRule.Pattern + "\"";
            }
        }

        producedRules.push_back(producedRule);
    }

    std::stringstream out;
    for (auto& producedRule : producedRules)
    {
        if (producedRule.Token == "")
        {
            producedRule.Token = illegalTokenName;
        }
        out << "\n    rules.emplace_back(" << producedRule.Token << ", "
            << (producedRule.Produces ? "true" : "false") << ", "
            << producedRule.Increment << ", " << producedRule.Pattern << ");";
    }

    std::string outStr = out.str();
    outStr.erase(0, 5); // Erase leading "\n    "
    return outStr;
}

/// <summary>
/// Replace $EOF_TOKEN.
/// </summary>
/// <param name="content">String to replace in.</param>
/// <param name="name">What to replace with.</param>
void ReplaceEofName(std::string& content, const std::string& name)
{
    Replace(content, "$EOF_TOKEN", name);
}

/// <summary>
/// Replace $INVALID_TOKEN.
/// </summary>
/// <param name="content">String to replace in.</param>
/// <param name="name">What to replace with.</param>
void ReplaceErrorName(std::string& content, const std::string& name)
{
    Replace(content, "$INVALID_TOKEN", name);
}

/// <summary>
/// Replace $LEXER_NAME.
/// </summary>
/// <param name="content">String to replace in.</param>
/// <param name="name">What to replace with.</param>
void ReplaceName(std::string& content, const std::string& name)
{
    Replace(content, "$LEXER_NAME", name);
}

/// <summary>
/// Replace $TOKEN_NAMES with list of token names.
/// </summary>
/// <param name="content">String to replace in.</param>
/// <param name="file">Lexer to extract token names from.</param>
/// <returns>
/// The name used for the end-of-file token and the name used for the error
/// token.
/// </returns>
std::tuple<std::string, std::string> ReplaceTokens(std::string& content,
                                                   FileNode file)
{
    std::set<std::string> tokenNames;
    GetTokenNames(file, tokenNames);

    std::string errorName = "PLEXIGLASS_NO_MATCH_TOKEN";
    tokenNames.insert(errorName);

    std::string eofName = "PLEXIGLASS_EOF";
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

/// <summary>
/// Replace $LEXER_RULES.
/// </summary>
/// <param name="content">String to replace in.</param>
/// <param name="file">The lexer to generate rules from.</param>
/// <param name="errorName">Name to use for error token.</param>
void ReplaceRules(std::string& content, FileNode file, std::string errorName)
{
    std::string ruleString = GetRuleString(file, errorName);
    Replace(content, "$LEXER_RULES", ruleString);
}

/// <summary>
/// Insert the ToString function into the template.
/// </summary>
/// <param name="content">String to replace in.</param>
/// <param name="file">The lexer to generate from.</param>
/// <param name="eofName">Name to use for end-of-file token.</param>
/// <param name="errorName">Name to use for error token.</param>
void ReplaceToString(std::string& content,
                     FileNode file,
                     std::string eofName,
                     std::string errorName)
{
    std::set<std::string> tokenNames;
    GetTokenNames(file, tokenNames);
    tokenNames.insert(eofName);
    tokenNames.insert(errorName);

    std::stringstream out;
    for (const std::string& name : tokenNames)
    {
        out << "case " << name << ":\n"
            << "        str = \"" << name << "\";\n"
            << "        break;\n"
            << "    ";
    }

    out << "default:\n            throw std::exception(\"Unrecognized token type in ToString()\");";

    Replace(content, "$TOKEN_TO_STRING", out.str());
}

/// <summary>
/// Replace $DEBUG_MODE.
/// </summary>
/// <param name="content">String to replace in.</param>
/// <param name="debug">Whether debug mode is enabled.</param>
void ReplaceDebug(std::string& content, bool debug)
{
    std::string replacement = (debug ? "1" : "0");
    Replace(content, "$DEBUG_MODE", replacement);
}

/// <summary>
/// Save a file.
/// </summary>
/// <param name="content">What to write.</param>
/// <param name="path">Where to write it.</param>
void SaveFile(const std::string& content, const std::filesystem::path& path)
{
    std::ofstream out(path);
    out << content;
}

/// <summary>
/// Generate a header from the template.
/// </summary>
/// <param name="file">The lexer to generate from.</param>
/// <param name="header">Path to the output header.</param>
/// <param name="name">Name of the lexer.</param>
/// <returns>
/// The name used for the end-of-file token and the name used for the error
/// token.
/// </returns>
std::tuple<std::string, std::string> TemplateHeader(
    FileNode file,
    std::filesystem::path header,
    std::string name)
{
    std::string content = header_template;

    ReplaceName(content, name);
    auto names = ReplaceTokens(content, file);

    SaveFile(content, header);

    return names;
}

/// <summary>
/// Generate a code file from the template.
/// </summary>
/// <param name="file">The lexer to generate from.</param>
/// <param name="eofName">Name to use for end-of-file token.</param>
/// <param name="errorName">Name to use for error token.</param>
/// <param name="code">Path to the output code file.</param>
/// <param name="name">Name of the lexer.</param>
/// <param name="debug">Whether to generate a debug mode lexer.</param>
void TemplateBody(FileNode file,
                  std::string eofName,
                  std::string errorName,
                  std::filesystem::path code,
                  std::string name,
                  bool debug)
{
    std::string content = code_template;

    ReplaceErrorName(content, errorName);
    ReplaceEofName(content, eofName);
    ReplaceName(content, name);
    ReplaceRules(content, file, eofName);
    ReplaceToString(content, file, eofName, errorName);
    ReplaceDebug(content, debug);
    SaveFile(content, code);
}

/// <summary>
/// Generate a header and code file from the template.
/// </summary>
/// <param name="file">The lexer to generate from.</param>
/// <param name="name">Name of the lexer.</param>
/// <param name="header">Path to the output header.</param>
/// <param name="code">Path to the output code file.</param>
/// <param name="debug">Whether to generate a debug mode lexer.</param>
void Template(FileNode file,
              std::string name,
              std::filesystem::path header,
              std::filesystem::path code,
              bool debug)
{
    std::filesystem::remove(header);
    std::filesystem::remove(code);
    auto [eofName, errorName] = TemplateHeader(file, header, name);
    TemplateBody(file, eofName, errorName, code, name, debug);
}
