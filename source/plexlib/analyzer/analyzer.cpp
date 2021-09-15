#include <analyzer/analyzer.hpp>

#include <map>

#include <error.hpp>

void Analyze(FileNode file)
{
	file->CheckDuplicateNames();
	file->CheckIllegalActions();
}

void _FileNode::CheckDuplicateNames()
{
	std::map<std::string, ExpressionNode> expressionMap;
	std::map<std::string, PatternNode> patternMap;

	for (auto& expression : m_expressions)
	{
		std::string name = expression->GetName();

		if (expressionMap.count(name) > 0)
		{
			DuplicateNameError(expression->GetLine(), expressionMap[name]->GetLine(), name);
		}
		else
		{
			expressionMap[name] = expression;
		}
	}

	for (auto& pattern : m_patterns)
	{
		std::string name = pattern->GetName();

		if (expressionMap.count(name) > 0)
		{
			DuplicateNameError(pattern->GetLine(), expressionMap[name]->GetLine(), name);
		}
		else if (patternMap.count(name) > 0)
		{
			DuplicateNameError(pattern->GetLine(), patternMap[name]->GetLine(), name);
		}
		else
		{
			patternMap[name] = pattern;
		}
	}
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
