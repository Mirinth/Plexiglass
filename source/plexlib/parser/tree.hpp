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
	void Add(RuleNode node);

private:
	std::vector<ExpressionNode> m_expressions;
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
	static PatternNode New();
	friend std::ostream& operator<<(std::ostream& out, PatternNode node);
};

class _RuleNode
{
public:
	static RuleNode New(std::string name);
	friend std::ostream& operator<<(std::ostream& out, RuleNode node);

	void Add(ActionNode node);

private:
	std::string m_name;
	std::vector<ActionNode> m_actions;
};

class _ActionNode
{
public:
	static ActionNode New(std::string name, std::string identifier = "");
	friend std::ostream& operator<<(std::ostream& out, ActionNode node);

private:
	std::string m_name;
	std::string m_identifier;
};
