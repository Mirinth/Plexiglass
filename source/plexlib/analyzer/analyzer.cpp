#include <analyzer/analyzer.hpp>

#include <map>

#include <error.hpp>

void Analyze(FileNode file)
{
	file->CheckDuplicateNames();
	file->CheckIllegalActions();
}

template <typename NodeType, typename MapType>
void CheckDuplicateNames(NodeType& nodes, MapType& map)
{
	for (auto& node : nodes)
	{
		std::string name = node->GetName();

		if (map.count(name) > 0)
		{
			size_t original = std::min(node->GetLine(), map[name]);
			size_t duplicate = std::max(node->GetLine(), map[name]);
			DuplicateNameError(duplicate, original, name);
		}
		else
		{
			map[name] = node->GetLine();
		}
	}
}

void _FileNode::CheckDuplicateNames()
{
	std::map<std::string, size_t> nameMap;

	::CheckDuplicateNames(m_expressions, nameMap);
	::CheckDuplicateNames(m_patterns, nameMap);
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
