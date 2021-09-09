#include <parser/parser.hpp>

#include <sstream>

#include <lexer/lexer.hpp>

ParseException::ParseException(const char* msg)
	: std::exception(msg)
{}

void File(Lexer& lexer);
void Keyword(Lexer& lexer);

void Error(std::string expected, const Token found);

void Parse(std::string_view data)
{
	Lexer lexer(data);
	File(lexer);
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
