#include <analyzer/analyzer.hpp>

#include <map>
#include <set>

#include <error.hpp>

void CheckDuplicateNames(FileNode node);
void CheckIllegalActions(FileNode node);
void CheckIllegalActions(RuleNode node);
void CheckIllegalActions(ActionNode node);
void CheckMissingNames(FileNode node);
void CheckMissingNames(PatternNode node, std::set<std::string>& names);
void CheckMissingNames(RuleNode node, std::set<std::string>& names);
void CheckMissingNames(IdentifierSequenceNode node, std::set<std::string>& names);

void Analyze(FileNode file)
{
    CheckDuplicateNames(file);
    CheckMissingNames(file);
    CheckIllegalActions(file);
    file->CheckIllegalStatements();
}

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

void CheckMissingNames(FileNode file)
{
    std::set<std::string> names;

    for (auto& expression : file->expressions)
    {
        names.insert(expression->name);
    }
    for (auto& pattern : file->patterns)
    {
        names.insert(pattern->name);
    }
    for (auto& pattern : file->patterns)
    {
        CheckMissingNames(pattern, names);
    }
    for (auto& rule : file->rules)
    {
        CheckMissingNames(rule, names);
    }
}

void CheckMissingNames(PatternNode node, std::set<std::string>& names)
{
    for (auto& sequence : node->sequences)
    {
        CheckMissingNames(sequence, names);
    }
}

void CheckMissingNames(IdentifierSequenceNode node, std::set<std::string>& names)
{
    for (auto& identifier : node->identifiers)
    {
        if (names.count(identifier) == 0)
        {
            MissingNameError(node->line, identifier);
        }
    }
}

void CheckMissingNames(RuleNode node, std::set<std::string>& names)
{
    if (names.count(node->name) == 0)
    {
        MissingNameError(node->line, node->name);
    }
}

void CheckIllegalActions(FileNode node)
{
    for (auto& rule : node->rules)
    {
        CheckIllegalActions(rule);
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

void CheckIllegalActions(RuleNode node)
{
    for (auto& action : node->actions)
    {
        CheckIllegalActions(action);
    }
}

void CheckIllegalActions(ActionNode node)
{
    if (node->name == "rewind")
    {
        Error(node->line, "'rewind' action not yet supported");
    }
    if (node->name == "transition")
    {
        Error(node->line, "'transition' action not yet supported");
    }
}
