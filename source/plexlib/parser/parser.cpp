#include <parser/parser.hpp>

#include <sstream>

#include <lexer/lexer.hpp>

ParseException::ParseException(const char* msg)
	: std::exception(msg)
{}

void Expression(Lexer& lexer);
void File(Lexer& lexer);
void Keyword(Lexer& lexer);

void Error(std::string expected, const Token found);
void Require(Lexer& lexer, std::string name, TokenType type, std::string value = "");

/// <summary>
/// Parse a block of data.
/// </summary>
/// <param name="data">The data to parse.</param>
void Parse(std::string_view data)
{
	Lexer lexer(data);
	File(lexer);
}

/// <summary>
/// Parse an expression statement.
/// </summary>
/// <param name="lexer">Lexer to parse from.</param>
void Expression(Lexer& lexer)
{
	Require(lexer, "'expression'", TokenType::Keyword, "expression");
	Require(lexer, "identifier", TokenType::Text);
	Require(lexer, "regular expression", TokenType::Regex);
}

/// <summary>
/// Parse a whole file.
/// </summary>
/// <param name="lexer">Lexer to parse from.</param>
void File(Lexer& lexer)
{
	while (lexer.Peek().type == TokenType::Keyword)
	{
		Keyword(lexer);
	}

	if (lexer.Peek().type == TokenType::Eof)
	{
		Error("keyword", lexer.Peek());
	}
}

/// <summary>
/// Parse a keyword.
/// </summary>
/// <param name="lexer">Lexer to parse from.</param>
void Keyword(Lexer& lexer)
{
	if (lexer.Peek().text == "expression")
	{
		Expression(lexer);
	}

	Error("'expression', 'pattern', or 'rule'", lexer.Peek());
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
void Require(Lexer& lexer, std::string name, TokenType type, std::string value /*= ""*/)
{
	if (lexer.Peek().type != type)
	{
		Error(name, lexer.Peek());
	}

	if (lexer.Peek().text != value && value != "")
	{
		Error(name, lexer.Peek());
	}

	lexer.Shift();
}
