#include <parser/tree.hpp>

FileNode _FileNode::New()
{
	return std::make_shared<_FileNode>();
}

std::ostream& operator<<(std::ostream& out, FileNode node)
{
	out << "File:\n"
		<< "\tExpressions:\n";

	for (auto& expression : node->m_expressions)
	{
		out << expression;
	}

	out << "\tRules:\n";
	for (auto& rule : node->m_rules)
	{
		out << rule;
	}

	return out;
}

void _FileNode::Add(ExpressionNode node)
{
	m_expressions.push_back(node);
}

void _FileNode::Add(RuleNode node)
{
	m_rules.push_back(node);
}

ExpressionNode _ExpressionNode::New(std::string name, std::string expression)
{
	ExpressionNode node = std::make_shared<_ExpressionNode>();
	node->m_name = name;
	node->m_expression = expression;
	return node;
}

std::ostream& operator<<(std::ostream& out, ExpressionNode node)
{
	out << "\t\t" << node->m_name << " : " << node->m_expression << "\n";
	return out;
}

RuleNode _RuleNode::New(std::string name)
{
	RuleNode node = std::make_shared<_RuleNode>();
	node->m_name = name;
	return node;
}

std::ostream& operator<<(std::ostream& out, RuleNode node)
{
	out << "\t\t" << node->m_name << " : \n";
	return out;
}
