#include <parser/tree.hpp>

#include <sstream>
#include <vector>

FileNode _FileNode::New()
{
    return std::make_shared<_FileNode>();
}

std::ostream& operator<<(std::ostream& out, const FileNode& node)
{
    out << "File:\n";

    out << "\tExpressions:\n";
    for (auto& expression : node->m_expressions)
    {
        out << expression;
    }

    out << "\tPatterns:\n";
    for (auto& pattern : node->m_patterns)
    {
        out << pattern;
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

void _FileNode::Add(PatternNode node)
{
    m_patterns.push_back(node);
}

void _FileNode::Add(RuleNode node)
{
    m_rules.push_back(node);
}

void _FileNode::GetTokenNames(std::set<std::string>& names) const
{
    for (const auto& rule : m_rules)
    {
        rule->GetTokenNames(names);
    }
}

std::string _FileNode::GetRuleString() const
{
    std::vector<Rule> rules;

    for (auto& rule : m_rules)
    {
        rules.push_back(rule->GetRule());
    }

    std::stringstream out;
    for (auto& rule : rules)
    {
        out << "\n\t\t{ " << (rule.Produces ? "true" : "false") << ", "
            << rule.Token << ", " << rule.Increment << ", " << rule.Pattern
            << "};";
    }

    return out.str();
}

ExpressionNode _ExpressionNode::New(size_t line,
                                    std::string name,
                                    std::string expression)
{
    ExpressionNode node = std::make_shared<_ExpressionNode>();
    node->m_line = line;
    node->m_name = name;
    node->m_expression = expression;
    return node;
}

std::ostream& operator<<(std::ostream& out, const ExpressionNode& node)
{
    out << "\t\t" << node->m_name << " : " << node->m_expression << "\n";
    return out;
}

size_t _ExpressionNode::GetLine() const
{
    return m_line;
}

std::string _ExpressionNode::GetName() const
{
    return m_name;
}

PatternNode _PatternNode::New(size_t line, std::string name)
{
    PatternNode node = std::make_shared<_PatternNode>();
    node->m_line = line;
    node->m_name = name;
    return node;
}

std::ostream& operator<<(std::ostream& out, const PatternNode& node)
{
    out << "\t\t" << node->m_name << " : ";

    for (size_t i = 0; i < node->m_sequences.size() - 1; i++)
    {
        out << node->m_sequences[i] << ", ";
    }

    out << node->m_sequences.back() << '\n';

    return out;
}

void _PatternNode::Add(IdentifierSequenceNode node)
{
    m_sequences.push_back(node);
}

size_t _PatternNode::GetLine() const
{
    return m_line;
}

std::string _PatternNode::GetName() const
{
    return m_name;
}

IdentifierSequenceNode _IdentifierSequenceNode::New(size_t line)
{
    IdentifierSequenceNode node = std::make_shared<_IdentifierSequenceNode>();
    node->m_line = line;
    return node;
}

std::ostream& operator<<(std::ostream& out, const IdentifierSequenceNode& node)
{
    for (size_t i = 0; i < node->m_identifiers.size() - 1; i++)
    {
        out << node->m_identifiers[i] << ' ';
    }

    out << node->m_identifiers.back();
    return out;
}

void _IdentifierSequenceNode::Add(std::string identifier)
{
    m_identifiers.push_back(identifier);
}

RuleNode _RuleNode::New(size_t line, std::string name)
{
    RuleNode node = std::make_shared<_RuleNode>();
    node->m_line = line;
    node->m_name = name;
    return node;
}

std::ostream& operator<<(std::ostream& out, const RuleNode& node)
{
    out << "\t\t" << node->m_name << " : ";

    for (size_t i = 0; i < node->m_actions.size() - 1; i++)
    {
        out << node->m_actions[i] << ", ";
    }

    out << node->m_actions.back() << "\n";
    return out;
}

void _RuleNode::Add(ActionNode node)
{
    m_actions.push_back(node);
}

void _RuleNode::GetTokenNames(std::set<std::string>& names) const
{
    for (const auto& action : m_actions)
    {
        action->GetTokenNames(names);
    }
}

Rule _RuleNode::GetRule() const
{
    Rule rule = { false, "", 0, m_name };
    
    for (const auto& action : m_actions)
    {
        action->GetRule(rule);
    }

    return rule;
}

ActionNode _ActionNode::New(size_t line,
                            std::string name,
                            std::string identifier /*= ""*/)
{
    ActionNode node = std::make_shared<_ActionNode>();
    node->m_line = line;
    node->m_name = name;
    node->m_identifier = identifier;
    return node;
}

std::ostream& operator<<(std::ostream& out, const ActionNode& node)
{
    out << node->m_name;
    if (!node->m_identifier.empty())
    {
        out << ' ' << node->m_identifier;
    }
    return out;
}

void _ActionNode::GetTokenNames(std::set<std::string>& names) const
{
    if (m_name == "produce")
    {
        names.insert(m_identifier);
    }
}

void _ActionNode::GetRule(Rule& rule)
{
    if (m_name == "produce-nothing")
    {
        rule.Produces = false;
    }
    else if (m_name == "produce")
    {
        rule.Produces = true;
        rule.Token = m_identifier;
    }
    else if (m_name == "++line" || m_name == "line++")
    {
        rule.Increment = 1;
    }
    else if (m_name == "--line" || m_name == "line--")
    {
        rule.Increment = -1;
    }
    else
    {
        throw std::exception("Illegal action name");
    }
}
