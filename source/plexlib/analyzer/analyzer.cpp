#include <analyzer/analyzer.hpp>

#include <map>
#include <set>

#include <error.hpp>

void Analyze(FileNode file)
{
    file->CheckDuplicateNames();
    file->CheckMissingNames();
    file->CheckIllegalActions();
    file->CheckIllegalStatements();
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

void _FileNode::CheckMissingNames()
{
    std::set<std::string> names;

    for (auto& expression : m_expressions)
    {
        names.insert(expression->GetName());
    }
    for (auto& pattern : m_patterns)
    {
        names.insert(pattern->GetName());
    }
    for (auto& pattern : m_patterns)
    {
        pattern->CheckMissingNames(names);
    }
    for (auto& rule : m_rules)
    {
        rule->CheckMissingNames(names);
    }
}

void _PatternNode::CheckMissingNames(std::set<std::string>& names)
{
    for (auto& sequence : m_sequences)
    {
        sequence->CheckMissingNames(names);
    }
}

void _IdentifierSequenceNode::CheckMissingNames(std::set<std::string>& names)
{
    for (auto& identifier : m_identifiers)
    {
        if (names.count(identifier) == 0)
        {
            MissingNameError(m_line, identifier);
        }
    }
}

void _RuleNode::CheckMissingNames(std::set<std::string>& names)
{
    if (names.count(m_name) == 0)
    {
        MissingNameError(m_line, m_name);
    }
}

void _FileNode::CheckIllegalActions()
{
    for (auto& rule : m_rules)
    {
        rule->CheckIllegalActions();
    }
}

void _FileNode::CheckIllegalStatements()
{
    if (m_patterns.size() > 0)
    {
        Error(m_patterns[0]->GetLine(),
              "'pattern' statement not yet supported");
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
