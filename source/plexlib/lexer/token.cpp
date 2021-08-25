#include <lexer/token.hpp>

Token::Token()
	: line(0), type(TokenType::Eof), text("")
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
	case TokenType::Action:
		name = "action";
		break;
	case TokenType::Alternator:
		name = "alternator";
		break;
	case TokenType::End:
		name = "end";
		break;
	case TokenType::Eof:
		name = "eof";
		break;
	case TokenType::Expression:
		name = "expression";
		break;
	case TokenType::Identifier:
		name = "identifier";
		break;
	case TokenType::Keyword:
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

	os << name << " " << token.text;
	return os;
}