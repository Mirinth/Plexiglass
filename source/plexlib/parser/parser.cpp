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

Token Require(Lexer& lexer, std::string name, TokenType type, std::string value = "");

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
	static std::set<std::string> unitActions = { "produce-nothing", "rewind", "++line", "line++", "--line", "line--" };
	static std::set<std::string> compositeActions = {"produce", "transition" };

	Require(lexer, "indent", TokenType::Indent);
	Token action = Require(lexer, "action", TokenType::Text);

	if (unitActions.count(action.text) > 0)
	{
		return _ActionNode::New(action.line, action.text);
	}
	else if (compositeActions.count(action.text) > 0)
	{
		if (lexer.Peek().type != TokenType::Text)
		{
			Error(action.line, "Expected identifier before end of line");
		}
		Token identifier = Require(lexer, "identifier", TokenType::Text);
		return _ActionNode::New(action.line, action.text, identifier.text);
	}
	else
	{
		Error("action", action);
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
	catch(std::regex_error&)
	{
		Error(expression.line, "Malformed regex.");
	}

	return _ExpressionNode::New(name.line, name.text, expression.text);
}

/// <summary>
/// Parse a whole file.
/// </summary>
/// <param name="lexer">Lexer to parse from.</param>
/// <returns>FileNode representing the parsed file.</returns>
FileNode File(Lexer& lexer)
{
	if (lexer.Peek().type == TokenType::Eof)
	{
		Error("keyword", lexer.Peek());
	}

	FileNode file = _FileNode::New();

	while (lexer.Peek().type != TokenType::Eof)
	{
		Token tok = Require(lexer, "'expression', 'pattern', or 'rule'", TokenType::Keyword);

		if (tok.text == "expression")
		{
			ExpressionNode node = Expression(lexer);
			file->Add(node);
		}
		else if (tok.text == "rule")
		{
			RuleNode node = Rule(lexer);
			file->Add(node);
		}
		else if (tok.text == "pattern")
		{
			PatternNode node = Pattern(lexer);
			file->Add(node);
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
/// <param name="initial">Whether this is the initial sequence or a followup one.
/// <returns>An IdentifierSequenceNode representing the parsed identifier sequence.</returns>
IdentifierSequenceNode IdentifierSequence(Lexer& lexer, bool initial)
{
	Require(lexer, "indent", TokenType::Indent);
	if (!initial)
	{
		Require(lexer, "alternator", TokenType::Alternator);
	}

	IdentifierSequenceNode sequence = _IdentifierSequenceNode::New();
	Token identifier = Require(lexer, "identifier", TokenType::Text);
	sequence->Add(identifier.text);

	while (lexer.Peek().type == TokenType::Text)
	{
		identifier = Require(lexer, "identifier", TokenType::Text);
		sequence->Add(identifier.text);
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
	PatternNode node = _PatternNode::New(name.line, name.text);

	IdentifierSequenceNode sequence = IdentifierSequence(lexer, true);
	node->Add(sequence);

	while (lexer.Peek().type == TokenType::Indent)
	{
		sequence = IdentifierSequence(lexer, false);
		node->Add(sequence);
	}

	return node;
}

/// <summary>
/// Parse a rule statement.
/// </summary>
/// <param name="lexer">Lexer to parse from.</param>
/// <returns>A RuleNode representing the parased rule.</returns>
RuleNode Rule(Lexer& lexer)
{
	Token name = Require(lexer, "identifier", TokenType::Text);
	RuleNode rule = _RuleNode::New(name.text);

	if (lexer.Peek().type != TokenType::Indent)
	{
		Error("indent", lexer.Peek());
	}

	while (lexer.Peek().type == TokenType::Indent)
	{
		ActionNode node = Action(lexer);
		rule->Add(node);
	}
	
	return rule;
}

/// <summary>
/// Shift a token out of the lexer if it matches the provided one,
/// or report an error if the wrong token is present.
/// </summary>
/// <param name="lexer">The lexer to require from.</param>
/// <param name="name">Name of what was expected. Used for error reporting.</param>
/// <param name="type">Type of token expected.</param>
/// <param name="value">Token text expected. If empty, any text is allowed.</param>
/// <returns>The token shifted.</returns>
Token Require(Lexer& lexer, std::string name, TokenType type, std::string value /*= ""*/)
{
	if (lexer.Peek().type != type)
	{
		Error(name, lexer.Peek());
	}

	if (lexer.Peek().text != value && value != "")
	{
		Error(name, lexer.Peek());
	}

	Token tok = lexer.Peek();
	lexer.Shift();
	return tok;
}
