#include <parser/tree.hpp>

#include <sstream>
#include <vector>

std::ostream& operator<<(std::ostream& out, const ExpressionNode& node);
std::ostream& operator<<(std::ostream& out, const PatternNode& node);
std::ostream& operator<<(std::ostream& out, const IdentifierSequenceNode& node);
std::ostream& operator<<(std::ostream& out, const RuleNode& node);
std::ostream& operator<<(std::ostream& out, const ActionNode& node);

FileNode NewFileNode()
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

ExpressionNode NewExpressionNode(size_t line,
                                 std::string name,
                                 std::string expression)
{
    _ExpressionNode node = { line, name, expression };
    return std::make_shared<_ExpressionNode>(node);
}

std::ostream& operator<<(std::ostream& out, const ExpressionNode& node)
{
    out << "\t\t" << node->name << " : " << node->expression << "\n";
    return out;
}

PatternNode NewPatternNode(size_t line, std::string name)
{
    _PatternNode node = { line, name };
    return std::make_shared<_PatternNode>(node);
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

IdentifierSequenceNode NewIdentifierSequenceNode(size_t line)
{
    _IdentifierSequenceNode node = { line };
    return std::make_shared<_IdentifierSequenceNode>(node);
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

RuleNode NewRuleNode(size_t line, std::string name)
{
    _RuleNode node = { line, name };
    return std::make_shared<_RuleNode>(node);
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

ActionNode NewActionNode(size_t line,
                         std::string name,
                         std::string identifier /*= ""*/)
{
    _ActionNode node = { line, name, identifier };
    return std::make_shared<_ActionNode>(node);
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
