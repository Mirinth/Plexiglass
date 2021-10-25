#pragma once

#include <memory>
#include <ostream>
#include <set>
#include <vector>

class _FileNode;
typedef std::shared_ptr<_FileNode> FileNode;

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
    friend std::ostream& operator<<(std::ostream& out, const FileNode& node);

    void Add(ExpressionNode node);
    void Add(PatternNode node);
    void Add(RuleNode node);

    void GetTokenNames(std::set<std::string>& names) const;
    std::string GetRuleString(std::string illegalTokenName) const;

    void CheckDuplicateNames();
    void CheckMissingNames();
    void CheckIllegalActions();
    void CheckIllegalStatements();

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
    friend std::ostream& operator<<(std::ostream& out,
                                    const ExpressionNode& node);

    size_t line;
    std::string name;
    std::string expression;
};

class _PatternNode
{
public:
    static PatternNode New(size_t line, std::string name);
    friend std::ostream& operator<<(std::ostream& out, const PatternNode& node);

    void Add(IdentifierSequenceNode node);

    void CheckMissingNames(std::set<std::string>& names);

    size_t line;
    std::string name;
    std::vector<IdentifierSequenceNode> sequences;
};

class _IdentifierSequenceNode
{
public:
    static IdentifierSequenceNode New(size_t line);
    friend std::ostream& operator<<(std::ostream& out,
                                    const IdentifierSequenceNode& node);

    void Add(std::string identifier);

    void CheckMissingNames(std::set<std::string>& names);

    size_t line;
    std::vector<std::string> identifiers;
};

class _RuleNode
{
public:
    static RuleNode New(size_t line, std::string name);
    friend std::ostream& operator<<(std::ostream& out, const RuleNode& node);

    void Add(ActionNode node);

    void GetTokenNames(std::set<std::string>& names) const;

    Rule GetRule() const;

    void CheckIllegalActions();
    void CheckMissingNames(std::set<std::string>& names);

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
    friend std::ostream& operator<<(std::ostream& out, const ActionNode& node);

    void GetTokenNames(std::set<std::string>& names) const;
    void GetRule(Rule& rule);

    void CheckIllegalActions();

    size_t line;
    std::string name;
    std::string identifier;
};
