#pragma once

#include <memory>
#include <ostream>
#include <set>
#include <vector>

class _FileNode;
typedef std::shared_ptr<_FileNode> FileNode;
std::ostream& operator<<(std::ostream& out, const FileNode& node);

class _ExpressionNode;
typedef std::shared_ptr<_ExpressionNode> ExpressionNode;

class _PatternNode;
typedef std::shared_ptr<_PatternNode> PatternNode;

class _IdentifierSequenceNode;
typedef std::shared_ptr<_IdentifierSequenceNode> IdentifierSequenceNode;

class _RuleNode;
typedef std::shared_ptr<_RuleNode> RuleNode;

class _ActionNode;
typedef std::shared_ptr<_ActionNode> ActionNode;

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
    static FileNode New();

    void GetTokenNames(std::set<std::string>& names) const;

    std::vector<ExpressionNode> expressions;
    std::vector<PatternNode> patterns;
    std::vector<RuleNode> rules;
};

class _ExpressionNode
{
public:
    static ExpressionNode New(size_t line,
                              std::string name,
                              std::string expression);

    size_t line;
    std::string name;
    std::string expression;
};

class _PatternNode
{
public:
    static PatternNode New(size_t line, std::string name);
    
    size_t line;
    std::string name;
    std::vector<IdentifierSequenceNode> sequences;
};

class _IdentifierSequenceNode
{
public:
    static IdentifierSequenceNode New(size_t line);
    
    size_t line;
    std::vector<std::string> identifiers;
};

class _RuleNode
{
public:
    static RuleNode New(size_t line, std::string name);
    
    void GetTokenNames(std::set<std::string>& names) const;

    size_t line;
    std::string name;
    std::vector<ActionNode> actions;
};

class _ActionNode
{
public:
    static ActionNode New(size_t line,
                          std::string name,
                          std::string identifier = "");
    
    void GetTokenNames(std::set<std::string>& names) const;

    size_t line;
    std::string name;
    std::string identifier;
};
