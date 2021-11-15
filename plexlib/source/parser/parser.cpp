#include <parser/parser.hpp>

#include <regex>
#include <set>

#include <error.hpp>
#include <lexer/lexer.hpp>
#include <lexer/token.hpp>

ActionNode Action(Lexer& lexer);
ExpressionNode Expression(Lexer& lexer);
FileNode File(Lexer& lexer);
IdentifierSequenceNode IdentifierSequence(Lexer& lexer, bool initial);
PatternNode Pattern(Lexer& lexer);
RuleNode Rule(Lexer& lexer);

Token Require(Lexer& lexer,
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
    Token action = Require(lexer, "action", TokenType::Text);

    if (unitActions.count(action.text) > 0)
    {
        return NewActionNode(action.line, action.text);
    }
    else if (compositeActions.count(action.text) > 0)
    {
        if (lexer.PeekToken() != TokenType::Text)
        {
            Error(action.line, "Expected identifier before end of line");
        }
        Token identifier = Require(lexer, "identifier", TokenType::Text);
        return NewActionNode(action.line, action.text, identifier.text);
    }
    else
    {
        Error("action", action.line, action.type, action.text);
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
    Token name = Require(lexer, "identifier", TokenType::Text);
    Require(lexer, "indent", TokenType::Indent);
    Token expression = Require(lexer, "regular expression", TokenType::Regex);

    try
    {
        std::regex dummy(expression.text);
    }
    catch (std::regex_error&)
    {
        Error(expression.line, "Malformed regex.");
    }

    return NewExpressionNode(name.line, name.text, expression.text);
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
        Token tok = Require(lexer, "'expression', 'pattern', or 'rule'",
                            TokenType::Keyword);

        if (tok.text == "expression")
        {
            ExpressionNode node = Expression(lexer);
            file->expressions.push_back(node);
        }
        else if (tok.text == "rule")
        {
            RuleNode node = Rule(lexer);
            file->rules.push_back(node);
        }
        else if (tok.text == "pattern")
        {
            PatternNode node = Pattern(lexer);
            file->patterns.push_back(node);
        }
        else
        {
            Error(tok.line, "Unrecognized keyword " + tok.text);
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
    Token indent = Require(lexer, "indent", TokenType::Indent);
    if (!initial)
    {
        Require(lexer, "alternator", TokenType::Alternator);
    }

    IdentifierSequenceNode sequence = NewIdentifierSequenceNode(indent.line);
    Token identifier = Require(lexer, "identifier", TokenType::Text);
    sequence->identifiers.push_back(identifier.text);

    while (lexer.PeekToken() == TokenType::Text)
    {
        identifier = Require(lexer, "identifier", TokenType::Text);
        sequence->identifiers.push_back(identifier.text);
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
    Token name = Require(lexer, "identifier", TokenType::Text);
    PatternNode node = NewPatternNode(name.line, name.text);

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
    Token name = Require(lexer, "identifier", TokenType::Text);
    RuleNode rule = NewRuleNode(name.line, name.text);

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
/// <returns>The token shifted.</returns>
Token Require(Lexer& lexer,
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

    Token tok(lexer.PeekLine(), lexer.PeekToken(), lexer.PeekText());
    lexer.Shift();
    return tok;
}
