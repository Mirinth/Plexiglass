#include <analyzer/analyzer.hpp>

#include <map>
#include <set>

#include <error.hpp>

template <typename NodeType, typename MapType>
void CheckDuplicateNames(NodeType& nodes, MapType& map)
{
    for (auto& node : nodes)
    {
        std::string name = node->name;

        if (map.count(name) > 0)
        {
            size_t original = std::min(node->line, map[name]);
            size_t duplicate = std::max(node->line, map[name]);
            DuplicateNameError(duplicate, original, name);
        }
        else
        {
            map[name] = node->line;
        }
    }
}

void CheckDuplicateNames(FileNode file)
{
    std::map<std::string, size_t> nameMap;

    ::CheckDuplicateNames(file->expressions, nameMap);
    ::CheckDuplicateNames(file->patterns, nameMap);
}

void _FileNode::CheckMissingNames()
{
    std::set<std::string> names;

    for (auto& expression : expressions)
    {
        names.insert(expression->name);
    }
    for (auto& pattern : patterns)
    {
        names.insert(pattern->name);
    }
    for (auto& pattern : patterns)
    {
        pattern->CheckMissingNames(names);
    }
    for (auto& rule : rules)
    {
        rule->CheckMissingNames(names);
    }
}

void _PatternNode::CheckMissingNames(std::set<std::string>& names)
{
    for (auto& sequence : sequences)
    {
        sequence->CheckMissingNames(names);
    }
}

void _IdentifierSequenceNode::CheckMissingNames(std::set<std::string>& names)
{
    for (auto& identifier : identifiers)
    {
        if (names.count(identifier) == 0)
        {
            MissingNameError(line, identifier);
        }
    }
}

void _RuleNode::CheckMissingNames(std::set<std::string>& names)
{
    if (names.count(name) == 0)
    {
        MissingNameError(line, name);
    }
}

void _FileNode::CheckIllegalActions()
{
    for (auto& rule : rules)
    {
        rule->CheckIllegalActions();
    }
}

void _FileNode::CheckIllegalStatements()
{
    if (patterns.size() > 0)
    {
        Error(patterns[0]->line,
              "'pattern' statement not yet supported");
    }
}

void _RuleNode::CheckIllegalActions()
{
    for (auto& action : actions)
    {
        action->CheckIllegalActions();
    }
}

void _ActionNode::CheckIllegalActions()
{
    if (name == "rewind")
    {
        Error(line, "'rewind' action not yet supported");
    }
    if (name == "transition")
    {
        Error(line, "'transition' action not yet supported");
    }
}

void Analyze(FileNode file)
{
    CheckDuplicateNames(file);
    file->CheckMissingNames();
    file->CheckIllegalActions();
    file->CheckIllegalStatements();
}
