#include <parser/tree.hpp>

#include <sstream>
#include <vector>

std::ostream& operator<<(std::ostream& out, const ExpressionNode& node);
std::ostream& operator<<(std::ostream& out, const PatternNode& node);
std::ostream& operator<<(std::ostream& out, const IdentifierSequenceNode& node);
std::ostream& operator<<(std::ostream& out, const RuleNode& node);
std::ostream& operator<<(std::ostream& out, const ActionNode& node);

/// <summary>
/// Create a new FileNode.
/// </summary>
/// <returns>FileNode.</returns>
FileNode NewFileNode()
{
    return std::make_shared<_FileNode>();
}

/// <summary>
/// Write a FileNode to a stream.
/// </summary>
/// <param name="out">Stream to write to.</param>
/// <param name="node">FileNode to write.</param>
/// <returns>ostream parameter.</returns>
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

/// <summary>
/// Create a new ExpressionNode.
/// </summary>
/// <param name="line">Line the expression starts on.</param>
/// <param name="name">Name of the expression.</param>
/// <param name="expression">The expression.</param>
/// <returns>ExpressionNode.</returns>
ExpressionNode NewExpressionNode(size_t line,
                                 std::string name,
                                 std::string expression)
{
    _ExpressionNode node = { line, name, expression };
    return std::make_shared<_ExpressionNode>(node);
}

/// <summary>
/// Write an ExpressionNode to a stream.
/// </summary>
/// <param name="out">Stream to write to.</param>
/// <param name="node">ExpressionNode to write.</param>
/// <returns>ostream parameter.</returns>
std::ostream& operator<<(std::ostream& out, const ExpressionNode& node)
{
    out << "\t\t" << node->name << " : " << node->expression << "\n";
    return out;
}

/// <summary>
/// Create a new ExpressionNode.
/// </summary>
/// <param name="line">Line the pattern starts on.</param>
/// <param name="name">Name of the pattern.</param>
/// <returns>PatternNode.</returns>
PatternNode NewPatternNode(size_t line, std::string name)
{
    _PatternNode node = { line, name };
    return std::make_shared<_PatternNode>(node);
}

/// <summary>
/// Write a PatternNode to a stream.
/// </summary>
/// <param name="out">Stream to write to.</param>
/// <param name="node">PatternNode to write.</param>
/// <returns>ostream parameter.</returns>
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

/// <summary>
/// Create a new IdentifierNode.
/// </summary>
/// <param name="line">Line the identifier starts on.</param>
/// <returns>IdentifierNode.</returns>
IdentifierSequenceNode NewIdentifierSequenceNode(size_t line)
{
    _IdentifierSequenceNode node = { line };
    return std::make_shared<_IdentifierSequenceNode>(node);
}

/// <summary>
/// Write an IdentifierSequenceNode to a stream.
/// </summary>
/// <param name="out">Stream to write to.</param>
/// <param name="node">IdentifierSequenceNode to write.</param>
/// <returns>ostream parameter.</returns>
std::ostream& operator<<(std::ostream& out, const IdentifierSequenceNode& node)
{
    for (size_t i = 0; i < node->identifiers.size() - 1; i++)
    {
        out << node->identifiers[i] << ' ';
    }

    out << node->identifiers.back();
    return out;
}

/// <summary>
/// Create a new RuleNode.
/// </summary>
/// <param name="line">Line the rule starts on.</param>
/// <param name="name">Name the rule refers to.</param>
/// <returns>RuleNode.</returns>
RuleNode NewRuleNode(size_t line, std::string name)
{
    _RuleNode node = { line, name };
    return std::make_shared<_RuleNode>(node);
}

/// <summary>
/// Write a RuleNode to a stream.
/// </summary>
/// <param name="out">Stream to write to.</param>
/// <param name="node">RuleNode to write.</param>
/// <returns>ostream parameter.</returns>
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

/// <summary>
/// Create a new ActionNode.
/// </summary>
/// <param name="line">Line the action starts on.</param>
/// <param name="name">Name of the action.</param>
/// <param name="identifier">The action's identifier, if any.</param>
/// <returns>ExpressionNode.</returns>
ActionNode NewActionNode(size_t line,
                         std::string name,
                         std::string identifier /*= ""*/)
{
    _ActionNode node = { line, name, identifier };
    return std::make_shared<_ActionNode>(node);
}

/// <summary>
/// Write an ActionNode to a stream.
/// </summary>
/// <param name="out">Stream to write to.</param>
/// <param name="node">ActionNode to write.</param>
/// <returns>ostream parameter.</returns>
std::ostream& operator<<(std::ostream& out, const ActionNode& node)
{
    out << node->name;
    if (!node->identifier.empty())
    {
        out << ' ' << node->identifier;
    }
    return out;
}
