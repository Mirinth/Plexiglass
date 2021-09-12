#include <parser/parser.hpp>

#include <regex>
#include <set>
#include <sstream>

#include <lexer/lexer.hpp>

ParseException::ParseException(const char* msg)
	: std::exception(msg)
{}

void Action(Lexer& lexer);
void Expression(Lexer& lexer);
FileNode File(Lexer& lexer);
void IdentifierSequence(Lexer& lexer, bool initial);
void Pattern(Lexer& lexer);
void Rule(Lexer& lexer);

void Error(size_t line, std::string message);
void Error(std::string expected, const Token found);
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
void Action(Lexer& lexer)
{
	static std::set<std::string> unitActions = { "produce-nothing", "rewind", "++line", "line++", "--line", "line--" };
	static std::set<std::string> compositeActions = {"produce", "transition" };

	Require(lexer, "indent", TokenType::Indent);
	Token action = Require(lexer, "action", TokenType::Text);

	if (unitActions.count(action.text) > 0)
	{
		return;
	}
	else if (compositeActions.count(action.text) > 0)
	{
		if (lexer.Peek().type != TokenType::Text)
		{
			Error(action.line, "Expected identifier before end of line");
		}
		Require(lexer, "identifier", TokenType::Text);
		return;
	}
	else
	{
		Error("action", action);
	}
}

/// <summary>
/// Parse an expression statement.
/// </summary>
/// <param name="lexer">Lexer to parse from.</param>
void Expression(Lexer& lexer)
{
	Require(lexer, "identifier", TokenType::Text);
	Require(lexer, "indent", TokenType::Indent);
	Token tok = Require(lexer, "regular expression", TokenType::Regex);

	try
	{
		std::regex dummy(tok.text);
	}
	catch(std::regex_error&)
	{
		Error(tok.line, "Malformed regex.");
	}
}

/// <summary>
/// Parse a whole file.
/// </summary>
/// <param name="lexer">Lexer to parse from.</param>
FileNode File(Lexer& lexer)
{
	if (lexer.Peek().type == TokenType::Eof)
	{
		Error("keyword", lexer.Peek());
	}

	while (lexer.Peek().type != TokenType::Eof)
	{
		Token tok = Require(lexer, "'expression', 'pattern', or 'rule'", TokenType::Keyword);

		if (tok.text == "expression")
		{
			Expression(lexer);
		}
		else if (tok.text == "rule")
		{
			Rule(lexer);
		}
		else if (tok.text == "pattern")
		{
			Pattern(lexer);
		}
		else
		{
			Error(tok.line, "Unrecognized keyword " + tok.text);
		}
	}

	return _FileNode::New();
}

/// <summary>
/// Parse a sequence of identifiers.
/// </summary>
/// <param name="lexer">Lexer to parse from.</param>
/// <param name="initial">Whether this is the initial sequence or a followup one.
void IdentifierSequence(Lexer& lexer, bool initial)
{
	Require(lexer, "indent", TokenType::Indent);
	if (!initial)
	{
		Require(lexer, "alternator", TokenType::Alternator);
	}
	Require(lexer, "identifier", TokenType::Text);

	while (lexer.Peek().type == TokenType::Text)
	{
		Require(lexer, "identifier", TokenType::Text);
	}
}

/// <summary>
/// Parse a pattern statement.
/// </summary>
/// <param name="lexer">Lexer to parse from.</param>
void Pattern(Lexer& lexer)
{
	Require(lexer, "identifier", TokenType::Text);
	
	IdentifierSequence(lexer, true);

	while (lexer.Peek().type == TokenType::Indent)
	{
		IdentifierSequence(lexer, false);
	}
}

/// <summary>
/// Parse a rule statement.
/// </summary>
/// <param name="lexer">Lexer to parse from.</param>
void Rule(Lexer& lexer)
{
	Require(lexer, "identifier", TokenType::Text);

	if (lexer.Peek().type != TokenType::Indent)
	{
		Error("indent", lexer.Peek());
	}

	while (lexer.Peek().type == TokenType::Indent)
	{
		Action(lexer);
	}
	/*Require(lexer, "regular expression", TokenType::Regex);*/
}

/// <summary>
/// Generate an error message and stop parsing.
/// </summary>
/// <param name="line">The line the error occurred on.</param>
/// <param name="message">The message to display.</param>
void Error(size_t line, std::string message)
{
	std::stringstream out;
	out << "Syntax error on line " << line
		<< ": " << message;
	throw ParseException(out.str().c_str());
}

/// <summary>
/// Generate an error message and stop parsing.
/// </summary>
/// <param name="expected">What was expected.</param>
/// <param name="tok">What was found.</param>
void Error(std::string expected, const Token tok)
{
	std::stringstream out;
	out << "Syntax error on line " << tok.line
		<< ": Expected " << expected
		<< " found " << tok.ToString();
	throw ParseException(out.str().c_str());
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
