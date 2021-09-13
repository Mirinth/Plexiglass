#pragma once

#include <ostream>
#include <memory>
#include <tuple>
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
	friend std::ostream& operator<<(std::ostream& out, FileNode node);

	void Add(ExpressionNode node);
	void Add(PatternNode node);
	void Add(RuleNode node);

	void CheckIllegalActions();

private:
	std::vector<ExpressionNode> m_expressions;
	std::vector<PatternNode> m_patterns;
	std::vector<RuleNode> m_rules;
};

class _ExpressionNode
{
public:
	static ExpressionNode New(std::string name, std::string expression);
	friend std::ostream& operator<<(std::ostream& out, ExpressionNode node);

private:
	std::string m_name;
	std::string m_expression;
};

class _PatternNode
{
public:
	static PatternNode New(std::string name);
	friend std::ostream& operator<<(std::ostream& out, PatternNode node);

	void Add(IdentifierSequenceNode node);

private:
	std::string m_name;
	std::vector<IdentifierSequenceNode> m_sequences;
};

class _IdentifierSequenceNode
{
public:
	static IdentifierSequenceNode New();
	friend std::ostream& operator<<(std::ostream& out, IdentifierSequenceNode node);

	void Add(std::string identifier);

private:
	std::vector<std::string> m_identifiers;
};

class _RuleNode
{
public:
	static RuleNode New(std::string name);
	friend std::ostream& operator<<(std::ostream& out, RuleNode node);

	void Add(ActionNode node);

	void CheckIllegalActions();

private:
	std::string m_name;
	std::vector<ActionNode> m_actions;
};

class _ActionNode
{
public:
	static ActionNode New(std::string name, std::string identifier = "");
	friend std::ostream& operator<<(std::ostream& out, ActionNode node);

	void CheckIllegalActions();

private:
	std::string m_name;
	std::string m_identifier;
};
