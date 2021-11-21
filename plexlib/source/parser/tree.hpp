#pragma once

#include <memory>
#include <ostream>
#include <set>
#include <vector>

struct _FileNode;
typedef std::shared_ptr<_FileNode> FileNode;
FileNode NewFileNode();
std::ostream& operator<<(std::ostream& out, const FileNode& node);

struct _ExpressionNode;
typedef std::shared_ptr<_ExpressionNode> ExpressionNode;
ExpressionNode NewExpressionNode(size_t line,
                                 std::string name,
                                 std::string expression);

struct _PatternNode;
typedef std::shared_ptr<_PatternNode> PatternNode;
PatternNode NewPatternNode(size_t line, std::string name);

struct _IdentifierSequenceNode;
typedef std::shared_ptr<_IdentifierSequenceNode> IdentifierSequenceNode;
IdentifierSequenceNode NewIdentifierSequenceNode(size_t line);

struct _RuleNode;
typedef std::shared_ptr<_RuleNode> RuleNode;
RuleNode NewRuleNode(size_t line, std::string name);

struct _ActionNode;
typedef std::shared_ptr<_ActionNode> ActionNode;
ActionNode NewActionNode(size_t line,
                         std::string name,
                         std::string identifier = "");

struct _FileNode
{
    std::vector<ExpressionNode> expressions;
    std::vector<PatternNode> patterns;
    std::vector<RuleNode> rules;
};

struct _ExpressionNode
{
    size_t line;
    std::string name;
    std::string expression;
};

struct _PatternNode
{
    size_t line;
    std::string name;
    std::vector<IdentifierSequenceNode> sequences;
};

struct _IdentifierSequenceNode
{
    size_t line;
    std::vector<std::string> identifiers;
};

struct _RuleNode
{
    size_t line;
    std::string name;
    std::vector<ActionNode> actions;
};

struct _ActionNode
{
    size_t line;
    std::string name;
    std::string identifier;
};
