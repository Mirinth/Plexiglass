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

std::string Token::ToString() const
{
	std::string str;
	switch (type)
	{
	case TokenType::ActDec:
	case TokenType::ActInc:
	case TokenType::ActProduce:
	case TokenType::ActProduceNil:
	case TokenType::ActRewind:
	case TokenType::ActTransition:
		str = "action";
		break;
	case TokenType::Alternator:
		str = "alternator";
		break;
	case TokenType::Eof:
		str = "eof";
		break;
	case TokenType::Regex:
		str = "expression";
		break;
	case TokenType::Identifier:
		str = "identifier";
		break;
	case TokenType::Indent:
		str = "indent";
		break;
	case TokenType::Keyword:
	case TokenType::KwdExpression:
	case TokenType::KwdPattern:
	case TokenType::KwdRule:
		str = "keyword";
		break;
	case TokenType::Text:
		str = "text";
		break;
	case TokenType::Unknown:
		str = "unknown";
		break;
	default:
		throw std::exception("Unrecognized token type in operator<<(Token)");
	}

	if (!text.empty())
	{
		str += " " + text;
	}

	return str;
}
