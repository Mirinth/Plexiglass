#pragma once

#include <ostream>
#include <memory>
#include <vector>

class _FileNode;
typedef std::shared_ptr<_FileNode> FileNode;

class _ExpressionNode;
typedef std::shared_ptr<_ExpressionNode> ExpressionNode;

class _FileNode
{
public:
	static FileNode New();
	friend std::ostream& operator<<(std::ostream& out, FileNode node);

	void Add(ExpressionNode node);

private:
	std::vector<ExpressionNode> m_expressions;
};

class _ExpressionNode
{
public:
	static ExpressionNode New();
	friend std::ostream& operator<<(std::ostream& out, ExpressionNode node);
};
