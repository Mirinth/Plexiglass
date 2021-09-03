#include <lexer/token.hpp>

Token::Token()
	: line(0), type(TokenType::Eof), text("")
{
}

Token::Token(TokenType type, std::string text /* = "" */)
	: line(0), type(type), text(text)
{
}

Token::Token(size_t line, TokenType type, std::string text)
	: line(line), type(type), text(text)
{
}

std::ostream& operator<<(std::ostream& os, const Token token)
{
	std::string name;
	switch (token.type)
	{
	case TokenType::ActDec:
	case TokenType::ActInc:
	case TokenType::ActProduce:
	case TokenType::ActProduceNil:
	case TokenType::ActRewind:
	case TokenType::ActTransition:
		name = "action";
		break;
	case TokenType::Alternator:
		name = "alternator";
		break;
	case TokenType::Eof:
		name = "eof";
		break;
	case TokenType::Regex:
		name = "expression";
		break;
	case TokenType::Identifier:
		name = "identifier";
		break;
	case TokenType::KwdExpression:
	case TokenType::KwdPattern:
	case TokenType::KwdRule:
		name = "keyword";
		break;
	case TokenType::Newline:
		name = "newline";
		break;
	case TokenType::Retry:
		name = "retry";
		break;
	case TokenType::Unknown:
		name = "unknown";
		break;
	default:
		throw std::exception("Unrecognized token type in operator<<(Token)");
	}

	os << name;
	if (!token.text.empty())
	{
		os << " " << token.text;
	}

	return os;
}