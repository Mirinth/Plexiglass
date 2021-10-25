#include <parser/tree.hpp>

#include <sstream>
#include <vector>

void Replace(std::string& subject,
             const std::string& find,
             const std::string& replace);

FileNode _FileNode::New()
{
    return std::make_shared<_FileNode>();
}

std::ostream& operator<<(std::ostream& out, const FileNode& node)
{
    out << "File:\n";

    out << "\tExpressions:\n";
    for (auto& expression : node->expressions)
    {
        out << expression;
    }

    out << "\tPatterns:\n";
    for (auto& pattern : node->patterns)
    {
        out << pattern;
    }

    out << "\tRules:\n";
    for (auto& rule : node->rules)
    {
        out << rule;
    }

    return out;
}

void _FileNode::GetTokenNames(std::set<std::string>& names) const
{
    for (const auto& rule : rules)
    {
        rule->GetTokenNames(names);
    }
}

std::string _FileNode::GetRuleString(std::string illegalTokenName) const
{
    std::vector<Rule> producedRules;

    for (auto& rule : rules)
    {
        Rule producedRule = rule->GetRule();

        for (auto& expression : expressions)
        {
            if (expression->name == producedRule.Pattern)
            {
                producedRule.Pattern = expression->expression;

                // Needs to come first so \ inserted by next one aren't escaped
                Replace(producedRule.Pattern, "\\", "\\\\"); // Escape backslaches
                Replace(producedRule.Pattern, "\"", "\\\""); // Escape double quotes

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

ExpressionNode _ExpressionNode::New(size_t line,
                                    std::string name,
                                    std::string expression)
{
    ExpressionNode node = std::make_shared<_ExpressionNode>();
    node->line = line;
    node->name = name;
    node->expression = expression;
    return node;
}

std::ostream& operator<<(std::ostream& out, const ExpressionNode& node)
{
    out << "\t\t" << node->name << " : " << node->expression << "\n";
    return out;
}

PatternNode _PatternNode::New(size_t line, std::string name)
{
    PatternNode node = std::make_shared<_PatternNode>();
    node->line = line;
    node->name = name;
    return node;
}

std::ostream& operator<<(std::ostream& out, const PatternNode& node)
{
    out << "\t\t" << node->name << " : ";

    for (size_t i = 0; i < node->sequences.size() - 1; i++)
    {
        out << node->sequences[i] << ", ";
    }

    out << node->sequences.back() << '\n';

    return out;
}

IdentifierSequenceNode _IdentifierSequenceNode::New(size_t line)
{
    IdentifierSequenceNode node = std::make_shared<_IdentifierSequenceNode>();
    node->line = line;
    return node;
}

std::ostream& operator<<(std::ostream& out, const IdentifierSequenceNode& node)
{
    for (size_t i = 0; i < node->identifiers.size() - 1; i++)
    {
        out << node->identifiers[i] << ' ';
    }

    out << node->identifiers.back();
    return out;
}

RuleNode _RuleNode::New(size_t line, std::string name)
{
    RuleNode node = std::make_shared<_RuleNode>();
    node->line = line;
    node->name = name;
    return node;
}

std::ostream& operator<<(std::ostream& out, const RuleNode& node)
{
    out << "\t\t" << node->name << " : ";

    for (size_t i = 0; i < node->actions.size() - 1; i++)
    {
        out << node->actions[i] << ", ";
    }

    out << node->actions.back() << "\n";
    return out;
}

void _RuleNode::GetTokenNames(std::set<std::string>& names) const
{
    for (const auto& action : actions)
    {
        action->GetTokenNames(names);
    }
}

Rule _RuleNode::GetRule() const
{
    Rule rule = { false, "", 0, name };
    
    for (const auto& action : actions)
    {
        action->GetRule(rule);
    }

    return rule;
}

ActionNode _ActionNode::New(size_t line,
                            std::string name,
                            std::string identifier /*= ""*/)
{
    ActionNode node = std::make_shared<_ActionNode>();
    node->line = line;
    node->name = name;
    node->identifier = identifier;
    return node;
}

std::ostream& operator<<(std::ostream& out, const ActionNode& node)
{
    out << node->name;
    if (!node->identifier.empty())
    {
        out << ' ' << node->identifier;
    }
    return out;
}

void _ActionNode::GetTokenNames(std::set<std::string>& names) const
{
    if (name == "produce")
    {
        names.insert(identifier);
    }
}

void _ActionNode::GetRule(Rule& rule)
{
    if (name == "produce-nothing")
    {
        rule.Produces = false;
    }
    else if (name == "produce")
    {
        rule.Produces = true;
        rule.Token = identifier;
    }
    else if (name == "++line" || name == "line++")
    {
        rule.Increment = 1;
    }
    else if (name == "--line" || name == "line--")
    {
        rule.Increment = -1;
    }
    else
    {
        throw std::exception("Illegal action name");
    }
}
