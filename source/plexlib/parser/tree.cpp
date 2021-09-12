#include <parser/tree.hpp>

FileNode _FileNode::New()
{
	return std::make_shared<_FileNode>();
}

std::ostream& operator<<(std::ostream& out, FileNode node)
{
	out << "File:\n";

	for (auto& expression : node->m_expressions)
	{
		out << expression;
	}

	return out;
}

void _FileNode::Add(ExpressionNode node)
{
	m_expressions.push_back(node);
}

ExpressionNode _ExpressionNode::New()
{
	return std::make_shared<_ExpressionNode>();
}

std::ostream& operator<<(std::ostream& out, ExpressionNode node)
{
	out << "\tExpression : \n";
	return out;
}
