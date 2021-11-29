#include <template/template.hpp>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>

#include <template-holder.hpp>
#include <utils.hpp>

constexpr char* eof_token = "__eof__";
constexpr char* jam_token = "__jam__";
constexpr char* nothing_token = "__nothing__";

constexpr char* initial_state = "__initial__";
constexpr char* jail_state = "__jail__";

struct TemplateRule
{
    std::string Active;     // state this rule is active in
    std::string Pattern;    // regex to match
    std::string Transition; // state this rule transitions to
    std::string Token;      // what gets produced (if anything)
    int Increment;          // how much to increment the line number by
};

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

    names.insert(eof_token);
    names.insert(jam_token);
    names.insert(nothing_token);
}

/// <summary>
/// Modify a TemplateRule's members to match an action.
/// </summary>
/// <param name="node">The action.</param>
/// <param name="rule">The rule.</param>
void GetRule(ActionNode node, TemplateRule& rule)
{
    if (node->name == "produce-nothing")
    {
        rule.Token = nothing_token;
    }
    else if (node->name == "produce")
    {
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
    else if (node->name == "transition")
    {
        rule.Transition = node->identifier;
    }
    else if (node->name == "state")
    {
        rule.Active = node->identifier;
    }
    else
    {
        throw std::exception("Illegal action name");
    }
}

/// <summary>
/// Get a TemplateRule for a RuleNode.
/// </summary>
/// <param name="node">RuleNode to get the TemplateRule for.</param>
/// <returns>TemplateRule for the RuleNode.</returns>
TemplateRule GetRule(RuleNode node)
{
    TemplateRule rule = { "", node->name, "", "", 0 };

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
/// <returns>The rule string.</returns>
std::string GetRuleString(FileNode node)
{
    std::vector<TemplateRule> producedRules;

    for (auto& rule : node->rules)
    {
        TemplateRule producedRule = GetRule(rule);
        producedRules.push_back(producedRule);
    }

    std::stringstream out;
    for (auto& producedRule : producedRules)
    {
        if (producedRule.Token == "")
        {
            producedRule.Token = nothing_token;
        }
        if (producedRule.Active == "")
        {
            producedRule.Active = initial_state;
        }
        if (producedRule.Transition == "")
        {
            producedRule.Transition = producedRule.Active;
        }
        out << "\n    __rules__.emplace_back(LexerState::"
            << producedRule.Active << ", " << producedRule.Pattern
            << ", LexerState::" << producedRule.Transition
            << ", TokenType::" << producedRule.Token << ", "
            << producedRule.Increment << ");";
    }

    std::string outStr = out.str();
    outStr.erase(0, 5); // Erase leading "\n    "
    return outStr;
}

/// <summary>
/// Replace $LEXER_STATES
/// </summary>
/// <param name="content">String to replace in.</param>
/// <param name="lexer">Lexer with state names.</param>
void ReplaceLexerStates(std::string& content, const FileNode lexer)
{
    std::set<std::string> states;

    for (const auto& rule : lexer->rules)
    {
        for (const auto& action : rule->actions)
        {
            if (action->name == "state")
            {
                states.insert(action->identifier);
            }
        }
    }

    std::stringstream names;
    names << initial_state << ",\n    ";

    for (const auto& state : states)
    {
        names << state << ",\n    ";
    }

    names << jail_state << ",";

    Replace(content, "$LEXER_STATES", names.str());
}

/// <summary>
/// Replace $TOKEN_NAMES with list of token names.
/// </summary>
/// <param name="content">String to replace in.</param>
/// <param name="file">Lexer to extract token names from.</param>
void ReplaceTokens(std::string& content, FileNode file)
{
    std::set<std::string> tokenNames;
    GetTokenNames(file, tokenNames);

    std::stringstream names;
    for (auto& tokenName : tokenNames)
    {
        names << "\n    " << tokenName << ",";
    }

    std::string namesStr = names.str();
    namesStr.erase(0, 5); // Erase the leading "\n    "

    Replace(content, "$TOKEN_NAMES", namesStr);
}

/// <summary>
/// Replace $EXPRESSIONS
/// </summary>
/// <param name="content">String to replace in.</param>
/// <param name="lexer">The lexer to generate rules from.</param>
void ReplaceExpressions(std::string& content, FileNode lexer)
{
    std::stringstream out;

    for (const auto& expressionNode : lexer->expressions)
    {
        const std::string name = expressionNode->name;
        std::string expression = expressionNode->expression;
        
        // Needs to come first so \ inserted by next one aren't escaped
        Replace(expression, "\\", "\\\\"); // Escape backslaches
        Replace(expression, "\"", "\\\""); // Escape double quotes

        out << "constexpr char* " << name << " = \"" << expression
            << "\";\n    ";
    }

    std::string outStr = out.str();
    outStr.erase(outStr.size() - 5, 5);

    Replace(content, "$EXPRESSIONS", outStr);
}

/// <summary>
/// Replace $LEXER_RULES.
/// </summary>
/// <param name="content">String to replace in.</param>
/// <param name="file">The lexer to generate rules from.</param>
void ReplaceRules(std::string& content, FileNode file)
{
    std::string ruleString = GetRuleString(file);
    Replace(content, "$LEXER_RULES", ruleString);
}

/// <summary>
/// Insert the ToString function into the template.
/// </summary>
/// <param name="content">String to replace in.</param>
/// <param name="file">The lexer to generate from.</param>
void ReplaceToString(std::string& content, FileNode file)
{
    std::set<std::string> tokenNames;
    GetTokenNames(file, tokenNames);

    std::stringstream out;
    for (const std::string& name : tokenNames)
    {
        out << "case TokenType::" << name << ":\n"
            << "        str = \"" << name << "\";\n"
            << "        break;\n"
            << "    ";
    }

    out << "default:\n            throw std::exception(\"Unrecognized token "
           "type in ToString()\");";

    Replace(content, "$TOKEN_TO_STRING", out.str());
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
void TemplateHeader(FileNode file,
                    std::filesystem::path header,
                    std::string name)
{
    std::string content = header_template;

    Replace(content, "$LEXER_NAME", name);
    ReplaceTokens(content, file);

    SaveFile(content, header);
}

/// <summary>
/// Generate a code file from the template.
/// </summary>
/// <param name="file">The lexer to generate from.</param>
/// <param name="code">Path to the output code file.</param>
/// <param name="name">Name of the lexer.</param>
/// <param name="debug">Whether to generate a debug mode lexer.</param>
void TemplateBody(FileNode file,
                  std::filesystem::path code,
                  std::string name,
                  bool debug)
{
    std::string content = code_template;

    Replace(content, "$EOF_TOKEN", eof_token);
    Replace(content, "$INVALID_TOKEN", jam_token);
    Replace(content, "$NOTHING_TOKEN", nothing_token);
    Replace(content, "$LEXER_NAME", name);
    ReplaceLexerStates(content, file);
    ReplaceExpressions(content, file);
    ReplaceRules(content, file);
    ReplaceToString(content, file);
    Replace(content, "$DEBUG_MODE", (debug ? "1" : "0"));
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
    TemplateHeader(file, header, name);
    TemplateBody(file, code, name, debug);
}
