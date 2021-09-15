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
	std::map<std::string, size_t> nameMap;

	for (auto& expression : m_expressions)
	{
		std::string name = expression->GetName();

		if (nameMap.count(name) > 0)
		{
			size_t original = std::min(expression->GetLine(), nameMap[name]);
			size_t duplicate = std::max(expression->GetLine(), nameMap[name]);
			DuplicateNameError(duplicate, original, name);
		}
		else
		{
			nameMap[name] = expression->GetLine();
		}
	}

	for (auto& pattern : m_patterns)
	{
		std::string name = pattern->GetName();

		if (nameMap.count(name) > 0)
		{
			size_t original = std::min(pattern->GetLine(), nameMap[name]);
			size_t duplicate = std::max(pattern->GetLine(), nameMap[name]);
			DuplicateNameError(duplicate, original, name);
		}
		else if (nameMap.count(name) > 0)
		{
			size_t original = std::min(pattern->GetLine(), nameMap[name]);
			size_t duplicate = std::max(pattern->GetLine(), nameMap[name]);
			DuplicateNameError(duplicate, original, name);
		}
		else
		{
			nameMap[name] = pattern->GetLine();
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
