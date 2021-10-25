#pragma once

#include <memory>
#include <ostream>
#include <set>
#include <vector>

class _FileNode;
typedef std::shared_ptr<_FileNode> FileNode;
FileNode NewFileNode();
std::ostream& operator<<(std::ostream& out, const FileNode& node);

class _ExpressionNode;
typedef std::shared_ptr<_ExpressionNode> ExpressionNode;
ExpressionNode NewExpressionNode(size_t line,
                                 std::string name,
                                 std::string expression);

class _PatternNode;
typedef std::shared_ptr<_PatternNode> PatternNode;
PatternNode NewPatternNode(size_t line, std::string name);

class _IdentifierSequenceNode;
typedef std::shared_ptr<_IdentifierSequenceNode> IdentifierSequenceNode;
IdentifierSequenceNode NewIdentifierSequenceNode(size_t line);

class _RuleNode;
typedef std::shared_ptr<_RuleNode> RuleNode;
RuleNode NewRuleNode(size_t line, std::string name);

class _ActionNode;
typedef std::shared_ptr<_ActionNode> ActionNode;
ActionNode NewActionNode(size_t line,
                         std::string name,
                         std::string identifier = "");
    

struct Rule
{
    bool Produces;       // whether anything is produced
    std::string Token;   // what gets produced (if anything)
    int Increment;       // how much to increment the line number by
    std::string Pattern; // regex to match
};

class _FileNode
{
public:
    std::vector<ExpressionNode> expressions;
    std::vector<PatternNode> patterns;
    std::vector<RuleNode> rules;
};

class _ExpressionNode
{
public:
    size_t line;
    std::string name;
    std::string expression;
};

class _PatternNode
{
public:
    size_t line;
    std::string name;
    std::vector<IdentifierSequenceNode> sequences;
};

class _IdentifierSequenceNode
{
public:
    size_t line;
    std::vector<std::string> identifiers;
};

class _RuleNode
{
public:
    size_t line;
    std::string name;
    std::vector<ActionNode> actions;
};

class _ActionNode
{
public:
    size_t line;
    std::string name;
    std::string identifier;
};
