#include <lexer/token.hpp>

/// <summary>
/// Get a human-readable string representation of a token.
/// </summary>
/// <param name="type">The token's type.</param>
/// <param name="text">The token's text.</param>
/// <returns>String representation of the token.</returns>
std::string ToString(TokenType type, const std::string& text)
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
