#include "lexer.hpp"

#include <map>
#include <regex>

#include <utils.hpp>

constexpr char* whitespace = " \t\r\n";

struct Rule
{
    /// <summary>
    /// Construct a Rule.
    /// </summary>
    /// <param name="token">The TokenType produced.</param>
    /// <param name="produce">Whether a token is produced at all.</param>
    /// <param name="increment">
    /// How much to change the current line number by.
    /// </param>
    /// <param name="pattern">
    /// A regular expression describing what the rule matches.
    /// </param>
    Rule(TokenType token, bool produce, int increment, const char* pattern)
        : Token(token), Produce(produce), Increment(increment), Pattern(pattern)
    {
    }

    TokenType Token;
    bool Produce;
    int Increment;
    std::regex Pattern;
};

/// <summary>
/// Get the rules to be used by the lexer.
/// </summary>
/// <returns>The rules to be used by the lexer.</returns>
std::vector<Rule> GetRules()
{
    std::vector<Rule> rules;

    return rules;
}

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
        throw std::exception("Unrecognized token type in ToString()");
    }

    if (!text.empty())
    {
        str += " " + text;
    }

    return str;
}

/// <summary>
/// Construct a lexer.
/// </summary>
/// <param name="data">The path to the file to lex.</param>
Lexer::Lexer(const std::filesystem::path& path)
{
    m_fileContent = ReadFile(path);
    m_view = m_fileContent;
    m_line = 1;
    Shift();
}

/// <summary>
/// Get the current token's line number.
/// </summary>
/// <returns>The current token's line number.</returns>
size_t Lexer::PeekLine() const
{
    return m_line;
}

/// <summary>
/// Get the current token's type.
/// </summary>
/// <returns>The current token's type.</returns>
TokenType Lexer::PeekToken() const
{
    return m_token;
}

/// <summary>
/// Get the curren token's text.
/// </summary>
/// <returns>The current token's text.</returns>
std::string Lexer::PeekText() const
{
    return m_text;
}

/// <summary>
/// Advance the lexer to the next token.
/// </summary>
void Lexer::Shift()
{
    bool success = false;
    while (!success)
    {
        success = ShiftHelper();
    }
}

/// <summary>
/// Helper function for $LEXER_NAME::Shift().
/// </summary>
/// <returns>
/// true if the found token should be used, false if it should be skipped.
/// </returns>
bool Lexer::ShiftHelper()
{
    if (m_view.empty())
    {
        m_token = TokenType::Eof;
        m_text = "";
        return true;
    }

    using vmatch = std::match_results<std::string_view::const_iterator>;
    static std::vector<Rule> rules = GetRules();

    size_t max_index = 0;
    size_t max_length = 0;
    std::string max_string;

    for (size_t index = 0; index < rules.size(); index++)
    {
        Rule rule = rules[index];
        vmatch m;
        bool matched =
            std::regex_search(m_view.begin(), m_view.end(), m, rule.Pattern);
        if (!matched || m.position() != 0)
        {
            continue;
        }

        // Ensure following cast is safe
        if (m.length() < 0)
        {
            throw std::exception("$LEXER_NAME::Shift(): Length was negative.");
        }
        size_t length = static_cast<size_t>(std::abs(m.length()));

        if (length > max_length)
        {
            max_length = length;
            max_index = index;
            max_string = m.str();
        }
    }

    if (max_length > 0 && rules[max_index].Produce)
    {
        m_token = rules[max_index].Token;
        m_text = m_view.substr(0, max_length);
        m_view.remove_prefix(max_length);
        m_line += rules[max_index].Increment;
        return true;
    }
    else if (max_length > 0)
    {
        m_view.remove_prefix(max_length);
        m_line += rules[max_index].Increment;
        return false;
    }
    else
    {
        m_token = TokenType::Unknown;
        m_text = "";
        m_view.remove_prefix(1);
        return true;
    }
}
