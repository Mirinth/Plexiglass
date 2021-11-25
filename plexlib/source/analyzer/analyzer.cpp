#include <analyzer/analyzer.hpp>

#include <map>
#include <set>

#include <error.hpp>

void CheckDuplicateActions(FileNode node);
void CheckDuplicateActions(RuleNode node);
void CheckDuplicateNames(FileNode node);
void CheckIllegalActions(FileNode node);
void CheckIllegalStatements(FileNode node);
void CheckMissingNames(FileNode node);
void CheckMissingNames(PatternNode node, std::set<std::string>& names);
void CheckMissingNames(RuleNode node, std::set<std::string>& names);
void CheckMissingNames(IdentifierSequenceNode node,
                       std::set<std::string>& names);
void CheckSelfTransitions(FileNode node);

bool ActionsEqual(const std::string& left, const std::string& right);

/// <summary>
/// Check whether a lexer is semantically valid.
/// </summary>
/// <param name="file">The lexer.</param>
void Analyze(FileNode file)
{
    CheckDuplicateNames(file);
    CheckDuplicateActions(file);
    CheckMissingNames(file);
    CheckIllegalActions(file);
    CheckIllegalStatements(file);
    CheckSelfTransitions(file);
}

/// <summary>
/// Check whether two actions are the same.
/// </summary>
/// <param name="left">The earlier action.</param>
/// <param name="right">The later action.</param>
/// <returns>True if the actions are the same.</returns>
bool ActionsEqual(const std::string& left, const std::string& right)
{
    if (left == "line++" || left == "++line")
    {
        if (right == "line++" || right == "++line")
        {
            return true;
        }
    }

    if (left == "line--" || left == "--line")
    {
        if (right == "line--" || right == "--line")
        {
            return true;
        }
    }

    return (left == right);
}

/// <summary>
/// Check whether a lexer contains rules with duplicate actions.
/// </summary>
/// <param name="node">The lexer.</param>
void CheckDuplicateActions(FileNode node)
{
    for (const auto& rule : node->rules)
    {
        CheckDuplicateActions(rule);
    }
}

/// <summary>
/// Check whether a rule contains duplicate actions.
/// </summary>
/// <param name="node">The rule.</param>
void CheckDuplicateActions(RuleNode node)
{
    for (size_t i = 0; i < node->actions.size(); i++)
    {
        for (size_t j = i + 1; j < node->actions.size(); j++)
        {
            if (ActionsEqual(node->actions[i]->name, node->actions[j]->name))
            {
                DuplicateActionError(node->actions[i]->line,
                                     node->actions[j]->line,
                                     node->actions[j]->name);
            }
        }
    }
}

/// <summary>
/// Check whether a container of nodes contains duplicate names.
/// </summary>
/// <typeparam name="NodeType">
/// Type of node container being checked.
/// </typeparam>
/// <param name="nodes">Nodes to check for duplicate names.</param>
/// <param name="map">
/// If populated, names in nodes are looked up in map to see if they're
/// duplicates. Names contained in nodes will be added to map.
/// </param>
template <typename NodeType>
void CheckDuplicateNames(NodeType& nodes, std::map<std::string, size_t>& map)
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

/// <summary>
/// Check whether a lexer contains duplicate names.
/// </summary>
/// <param name="file">The lexer.</param>
void CheckDuplicateNames(FileNode file)
{
    std::map<std::string, size_t> nameMap;

    ::CheckDuplicateNames(file->expressions, nameMap);
    ::CheckDuplicateNames(file->patterns, nameMap);
}

/// <summary>
/// Check if a lexer refers to undefined names.
/// </summary>
/// <param name="file">The lexer.</param>
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

/// <summary>
/// Check if a pattern refers to an undefined name.
/// </summary>
/// <param name="node">The pattern.</param>
/// <param name="names">Defined names.</param>
void CheckMissingNames(PatternNode node, std::set<std::string>& names)
{
    for (auto& sequence : node->sequences)
    {
        CheckMissingNames(sequence, names);
    }
}

/// <summary>
/// Check if an identifier sequence refers to an undefined name.
/// </summary>
/// <param name="node">The identifier sequence.</param>
/// <param name="names">Defined names.</param>
void CheckMissingNames(IdentifierSequenceNode node,
                       std::set<std::string>& names)
{
    for (auto& identifier : node->identifiers)
    {
        if (names.count(identifier) == 0)
        {
            MissingNameError(node->line, identifier);
        }
    }
}

/// <summary>
/// Check if a rule transitions to itself.
/// </summary>
/// <param name="node">The lexer.</param>
void CheckSelfTransitions(FileNode node)
{
    for (const auto& rule : node->rules)
    {
        bool foundTarget = false;
        std::string state = "__initial__";
        std::string target = "__initial__";
        size_t line = 0;

        for (const auto& action : rule->actions)
        {
            if (action->name == "state")
            {
                state = action->identifier;
            }
            if (action->name == "transition")
            {
                foundTarget = true;
                target = action->identifier;
                line = action->line;
            }
        }

        if (foundTarget && state == target)
        {
            Error(line, "Rule transitions to its own state.");
        }
    }
}

/// <summary>
/// Check if a rule refers to an undefined name.
/// </summary>
/// <param name="node">The rule.</param>
/// <param name="names">Defined names.</param>
void CheckMissingNames(RuleNode node, std::set<std::string>& names)
{
    if (names.count(node->name) == 0)
    {
        MissingNameError(node->line, node->name);
    }
}

/// <summary>
/// Check whether a lexer uses actions that are syntactically allowed but
/// semantically forbidden.
/// </summary>
/// <param name="node">The lexer.</param>
void CheckIllegalActions(FileNode node)
{
    for (auto& rule : node->rules)
    {
        for (auto& action : rule->actions)
        {
            if (action->name == "rewind")
            {
                Error(action->line, "'rewind' action not yet supported");
            }
        }
    }
}

/// <summary>
/// Check whether a lexer uses statements that are syntactically allowed but
/// semantically forbidden. 
/// </summary>
/// <param name="node">The lexer.</param>
void CheckIllegalStatements(FileNode node)
{
    if (node->patterns.size() > 0)
    {
        Error(node->patterns[0]->line, "'pattern' statement not yet supported");
    }
}
