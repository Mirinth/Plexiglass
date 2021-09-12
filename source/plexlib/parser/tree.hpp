#pragma once

#include <ostream>
#include <memory>
#include <tuple>
#include <vector>

class _FileNode;
typedef std::shared_ptr<_FileNode> FileNode;

class _ExpressionNode;
typedef std::shared_ptr<_ExpressionNode> ExpressionNode;

class _RuleNode;
typedef std::shared_ptr<_RuleNode> RuleNode;

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

class _RuleNode
{
public:
	static RuleNode New(std::string name);
	friend std::ostream& operator<<(std::ostream& out, RuleNode node);

private:
	std::string m_name;
};
