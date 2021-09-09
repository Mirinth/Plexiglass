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

void Parse(std::string_view data)
{
	Lexer lexer(data);
	File(lexer);
}

void Expression(Lexer& lexer)
{
	Require(lexer, "'expression'", TokenType::Keyword, "expression");
	Require(lexer, "identifier", TokenType::Text);
	Require(lexer, "regular expression", TokenType::Regex);
}

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

void Keyword(Lexer& lexer)
{
	if (lexer.Peek().text == "expression")
	{
		Expression(lexer);
	}

	Error("'expression', 'pattern', or 'rule'", lexer.Peek());
}

void Error(std::string expected, const Token tok)
{
	std::stringstream out;
	out << "Syntax error on line " << tok.line
		<< ": Expected " << expected
		<< " found " << tok.ToString();
	throw ParseException(out.str().c_str());
}

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
