#include <parser/tree.hpp>

FileNode _FileNode::New()
{
	return std::make_shared<_FileNode>();
}

std::ostream& operator<<(std::ostream& out, FileNode node)
{
	out << "File:\n";

	out << "\tExpressions:\n";
	for (auto& expression : node->m_expressions)
	{
		out << expression;
	}

	out << "\tPatterns:\n";

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

void _FileNode::Add(PatternNode node)
{
	m_patterns.push_back(node);
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

PatternNode _PatternNode::New()
{
	PatternNode node = std::make_shared<_PatternNode>();
	return node;
}

std::ostream& operator<<(std::ostream& out, PatternNode node)
{
	out << "\t\t" << "Dummy pattern node" << "\n";
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
	out << "\t\t" << node->m_name << " : ";

	for (size_t i = 0; i < node->m_actions.size() - 1; i++)
	{
		out << node->m_actions[i] << ", ";
	}

	out << node->m_actions[node->m_actions.size() - 1] << "\n";

	return out;
}

void _RuleNode::Add(ActionNode node)
{
	m_actions.push_back(node);
}

ActionNode _ActionNode::New(std::string name, std::string identifier /*= ""*/)
{
	ActionNode node = std::make_shared<_ActionNode>();
	node->m_name = name;
	node->m_identifier = identifier;
	return node;
}

std::ostream& operator<<(std::ostream& out, ActionNode node)
{
	out << node->m_name;
	if (!node->m_identifier.empty())
	{
		out << ' ' << node->m_identifier;
	}
	return out;
}