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

class _FileNode
{
public:
    static FileNode New();
    friend std::ostream& operator<<(std::ostream& out, const FileNode& node);

    void Add(ExpressionNode node);
    void Add(PatternNode node);
    void Add(RuleNode node);

    void GetTokenNames(std::set<std::string>& names) const;
    std::string GetRuleString() const;

    void CheckDuplicateNames();
    void CheckMissingNames();
    void CheckIllegalActions();
    void CheckIllegalStatements();

private:
    std::vector<ExpressionNode> m_expressions;
    std::vector<PatternNode> m_patterns;
    std::vector<RuleNode> m_rules;
};

class _ExpressionNode
{
public:
    static ExpressionNode New(size_t line,
                              std::string name,
                              std::string expression);
    friend std::ostream& operator<<(std::ostream& out,
                                    const ExpressionNode& node);

    size_t GetLine() const;
    std::string GetName() const;

private:
    size_t m_line;
    std::string m_name;
    std::string m_expression;
};

class _PatternNode
{
public:
    static PatternNode New(size_t line, std::string name);
    friend std::ostream& operator<<(std::ostream& out, const PatternNode& node);

    void Add(IdentifierSequenceNode node);

    size_t GetLine() const;
    std::string GetName() const;

    void CheckMissingNames(std::set<std::string>& names);

private:
    size_t m_line;
    std::string m_name;
    std::vector<IdentifierSequenceNode> m_sequences;
};

class _IdentifierSequenceNode
{
public:
    static IdentifierSequenceNode New(size_t line);
    friend std::ostream& operator<<(std::ostream& out,
                                    const IdentifierSequenceNode& node);

    void Add(std::string identifier);

    void CheckMissingNames(std::set<std::string>& names);

private:
    size_t m_line;
    std::vector<std::string> m_identifiers;
};

class _RuleNode
{
public:
    static RuleNode New(size_t line, std::string name);
    friend std::ostream& operator<<(std::ostream& out, const RuleNode& node);

    void Add(ActionNode node);

    void GetTokenNames(std::set<std::string>& names) const;

    void CheckIllegalActions();
    void CheckMissingNames(std::set<std::string>& names);

private:
    size_t m_line;
    std::string m_name;
    std::vector<ActionNode> m_actions;
};

class _ActionNode
{
public:
    static ActionNode New(size_t line,
                          std::string name,
                          std::string identifier = "");
    friend std::ostream& operator<<(std::ostream& out, const ActionNode& node);

    void GetTokenNames(std::set<std::string>& names) const;

    void CheckIllegalActions();

private:
    size_t m_line;
    std::string m_name;
    std::string m_identifier;
};
