#include <lexer/token.hpp>

/// <summary>
/// Construct an invalid token.
/// </summary>
Token::Token()
	: line(0), type(TokenType::Unknown), text("")
{
}

/// <summary>
/// Construct a token.
/// </summary>
/// <param name="line">The line number the token occurred on.</param>
/// <param name="type">The token's type.</param>
/// <param name="text">The token's text.</param>
Token::Token(size_t line, TokenType type, std::string text /* = "" */)
	: line(line), type(type), text(text)
{
}

/// <summary>
/// Get a human-readable string representation of the token.
/// </summary>
/// <returns>String representation of the token.</returns>
std::string Token::ToString() const
{
	std::string str;
	switch (type)
	{
	case TokenType::Alternator:
		str = "alternator";
		break;
	case TokenType::Eof:
		str = "eof";
		break;
	case TokenType::Regex:
		str = "expression";
		break;
	case TokenType::Indent:
		str = "indent";
		break;
	case TokenType::Keyword:
		str = "keyword";
		break;
	case TokenType::Text:
		str = "text";
		break;
	case TokenType::Unknown:
		str = "unknown";
		break;
	default:
		throw std::exception("Unrecognized token type in Token::ToString()");
	}

	if (!text.empty())
	{
		str += " " + text;
	}

	return str;
}
