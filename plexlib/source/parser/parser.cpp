#include <parser/parser.hpp>

#include <regex>
#include <set>

#include <error.hpp>
#include <lexer/lexer.hpp>

ActionNode Action(Lexer& lexer);
ExpressionNode Expression(Lexer& lexer);
FileNode File(Lexer& lexer);
IdentifierSequenceNode IdentifierSequence(Lexer& lexer, bool initial);
PatternNode Pattern(Lexer& lexer);
RuleNode Rule(Lexer& lexer);

std::string Require(Lexer& lexer,
              std::string name,
              TokenType type,
              std::string value = "");

/// <summary>
/// Parse a block of data.
/// </summary>
/// <param name="data">The data to parse.</param>
/// <returns>A FileNode representing the file.</returns>
FileNode Parse(std::string_view data)
{
    Lexer lexer(data);
    return File(lexer);
}

/// <summary>
/// Parse an action.
/// </summary>
/// <param name="lexer">Lexer to parse from.</param>
/// <returns>An ActionNode representing the parsed action.</returns>
ActionNode Action(Lexer& lexer)
{
    static std::set<std::string> unitActions = { "produce-nothing", "rewind",
                                                 "++line",          "line++",
                                                 "--line",          "line--" };
    static std::set<std::string> compositeActions = { "produce", "transition" };

    Require(lexer, "indent", TokenType::Indent);
    size_t line = lexer.PeekLine();
    TokenType token = lexer.PeekToken();
    std::string action = Require(lexer, "action", TokenType::Text);

    if (unitActions.count(action) > 0)
    {
        return NewActionNode(line, action);
    }
    else if (compositeActions.count(action) > 0)
    {
        if (lexer.PeekToken() != TokenType::Text)
        {
            Error(line, "Expected identifier before end of line");
        }
        std::string identifier = Require(lexer, "identifier", TokenType::Text);
        return NewActionNode(line, action, identifier);
    }
    else
    {
        Error("action", line, token, action);
        return nullptr; // Silence warning; Error never returns.
    }
}

/// <summary>
/// Parse an expression statement.
/// </summary>
/// <param name="lexer">Lexer to parse from.</param>
/// <returns>ExpressionNode representing the parsed expression.</returns>
ExpressionNode Expression(Lexer& lexer)
{
    size_t nameLine = lexer.PeekLine();
    std::string name = Require(lexer, "identifier", TokenType::Text);
    size_t expressionLine = lexer.PeekLine();
    Require(lexer, "indent", TokenType::Indent);
    std::string expression = Require(lexer, "regular expression", TokenType::Regex);

    try
    {
        std::regex dummy(expression);
    }
    catch (std::regex_error&)
    {
        Error(expressionLine, "Malformed regex.");
    }

    return NewExpressionNode(nameLine, name, expression);
}

/// <summary>
/// Parse a whole file.
/// </summary>
/// <param name="lexer">Lexer to parse from.</param>
/// <returns>FileNode representing the parsed file.</returns>
FileNode File(Lexer& lexer)
{
    if (lexer.PeekToken() == TokenType::Eof)
    {
        Error("keyword", lexer.PeekLine(), lexer.PeekToken(), lexer.PeekText());
    }

    FileNode file = NewFileNode();

    while (lexer.PeekToken() != TokenType::Eof)
    {
        size_t line = lexer.PeekLine();
        std::string keyword = Require(lexer, "'expression', 'pattern', or 'rule'",
                            TokenType::Keyword);

        if (keyword == "expression")
        {
            ExpressionNode node = Expression(lexer);
            file->expressions.push_back(node);
        }
        else if (keyword == "rule")
        {
            RuleNode node = Rule(lexer);
            file->rules.push_back(node);
        }
        else if (keyword == "pattern")
        {
            PatternNode node = Pattern(lexer);
            file->patterns.push_back(node);
        }
        else
        {
            Error(line, "Unrecognized keyword " + keyword);
        }
    }

    return file;
}

/// <summary>
/// Parse a sequence of identifiers.
/// </summary>
/// <param name="lexer">Lexer to parse from.</param>
/// <param name="initial">
/// Whether this is the initial sequence or a followup one.
/// </param>
/// <returns>
/// An IdentifierSequenceNode representing the parsed identifier sequence.
/// </returns>
IdentifierSequenceNode IdentifierSequence(Lexer& lexer, bool initial)
{
    size_t line = lexer.PeekLine();
    std::string indent = Require(lexer, "indent", TokenType::Indent);
    if (!initial)
    {
        Require(lexer, "alternator", TokenType::Alternator);
    }

    IdentifierSequenceNode sequence = NewIdentifierSequenceNode(line);
    std::string identifier = Require(lexer, "identifier", TokenType::Text);
    sequence->identifiers.push_back(identifier);

    while (lexer.PeekToken() == TokenType::Text)
    {
        identifier = Require(lexer, "identifier", TokenType::Text);
        sequence->identifiers.push_back(identifier);
    }

    return sequence;
}

/// <summary>
/// Parse a pattern statement.
/// </summary>
/// <param name="lexer">Lexer to parse from.</param>
/// <returns>A PatternNode representing the parsed pattern.</returns>
PatternNode Pattern(Lexer& lexer)
{
    size_t line = lexer.PeekLine();
    std::string name = Require(lexer, "identifier", TokenType::Text);
    PatternNode node = NewPatternNode(line, name);

    IdentifierSequenceNode sequence = IdentifierSequence(lexer, true);
    node->sequences.push_back(sequence);

    while (lexer.PeekToken() == TokenType::Indent)
    {
        sequence = IdentifierSequence(lexer, false);
        node->sequences.push_back(sequence);
    }

    return node;
}

/// <summary>
/// Parse a rule statement.
/// </summary>
/// <param name="lexer">Lexer to parse from.</param>
/// <returns>A RuleNode representing the parsed rule.</returns>
RuleNode Rule(Lexer& lexer)
{
    size_t line = lexer.PeekLine();
    std::string name = Require(lexer, "identifier", TokenType::Text);
    RuleNode rule = NewRuleNode(line, name);

    if (lexer.PeekToken() != TokenType::Indent)
    {
        Error("indent", lexer.PeekLine(), lexer.PeekToken(), lexer.PeekText());
    }

    while (lexer.PeekToken() == TokenType::Indent)
    {
        ActionNode node = Action(lexer);
        rule->actions.push_back(node);
    }

    return rule;
}

/// <summary>
/// Shift a token out of the lexer if it matches the provided one,
/// or report an error if the wrong token is present.
/// </summary>
/// <param name="lexer">The lexer to require from.</param>
/// <param name="name">
/// Name of what was expected. Used for error reporting.
/// </param>
/// <param name="type">Type of token expected.</param>
/// <param name="value">
/// Token text expected. If empty, any text is allowed.
/// </param>
/// <returns>The text of the token shifted.</returns>
std::string Require(Lexer& lexer,
              std::string name,
              TokenType type,
              std::string value /*= ""*/)
{
    if (lexer.PeekToken() != type)
    {
        Error(name, lexer.PeekLine(), lexer.PeekToken(), lexer.PeekText());
    }

    if (lexer.PeekText() != value && value != "")
    {
        Error(name, lexer.PeekLine(), lexer.PeekToken(), lexer.PeekText());
    }

    std::string text = lexer.PeekText();
    lexer.Shift();
    return text;
}
