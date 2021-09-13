#include <analyzer/analyzer.hpp>

#include <map>

#include <error.hpp>

void Analyze(FileNode file)
{
	file->CheckIllegalActions();
}

void _FileNode::CheckIllegalActions()
{
	for (auto& rule : m_rules)
	{
		rule->CheckIllegalActions();
	}
}

void _RuleNode::CheckIllegalActions()
{
	for (auto& action : m_actions)
	{
		action->CheckIllegalActions();
	}
}

void _ActionNode::CheckIllegalActions()
{
	if (m_name == "rewind")
	{
		Error(m_line, "'rewind' action not yet supported");
	}
	if (m_name == "transition")
	{
		Error(m_line, "'transition' action not yet supported");
	}
}
