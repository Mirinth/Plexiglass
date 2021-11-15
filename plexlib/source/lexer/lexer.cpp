#include "lexer.hpp"

#include <map>

constexpr char* whitespace = " \t\r\n";

bool StartsWith(const std::string_view& toSearch, const std::string find);
std::string_view StripComment(std::string_view line);
std::string_view StripWhitespace(std::string_view line);
bool IsBlank(std::string_view line);

/// <summary>
/// Construct a lexer.
/// </summary>
/// <param name="data">The data to lex.</param>
Lexer::Lexer(std::string_view data)
    : m_data(data), m_lineNumber(0), m_expectExpression(false)
{
    FillBuffer();
}

/// <summary>
/// Get the current token's line number.
/// </summary>
/// <returns>The current token's line number.</returns>
size_t Lexer::PeekLine() const
{
    return m_LineNumberBuffer.front();
}

/// <summary>
/// Get the current token's type.
/// </summary>
/// <returns>The current token's type.</returns>
TokenType Lexer::PeekToken() const
{
    return m_TokenTypeBuffer.front();
}
 
/// <summary>
/// Get the curren token's text.
/// </summary>
/// <returns>The current token's text.</returns>
std::string Lexer::PeekText() const
{
    return m_TokenTextBuffer.front();
}

/// <summary>
/// Advance the lexer to the next token.
/// </summary>
void Lexer::Shift()
{
    m_LineNumberBuffer.pop();
    m_TokenTypeBuffer.pop();
    m_TokenTextBuffer.pop();
    FillBuffer();
}

/// <summary>
/// Lex a single token's text out of a line. Assumes line has something to lex.
/// </summary>
/// <param name="line">Line to lex from. Token is stripped from line.</param>
/// <returns>The lexed token's text.</returns>
std::string Lexer::LexToken(std::string_view& line)
{
    size_t space = line.find_first_of(whitespace);
    std::string text;
    if (space == std::string_view::npos)
    {
        text = std::string(line);
        line = std::string_view();
    }
    else
    {
        text = std::string(line.substr(0, space));
        line.remove_prefix(space + 1); // Remove the space too
    }

    return text;
}

/// <summary>
/// Get the first line in m_data. After returning, m_data will be modified
/// to remove the returned line from its content.
/// </summary>
/// <returns>
/// The first line in m_data. All lines but the last will have a trailing
/// newline. May be empty if m_data is empty.
/// </returns>
std::string_view Lexer::GetLine()
{
    if (m_data.empty())
    {
        return std::string_view();
    }

    m_lineNumber++;

    std::string_view line;
    size_t size = m_data.find('\n');
    if (size == std::string_view::npos)
    {
        line = m_data;
        m_data.remove_prefix(m_data.size());
    }
    else
    {
        line = m_data.substr(0, size + 1); // include the newline
        m_data.remove_prefix(size + 1);
    }

    return line;
}

/// <summary>
/// Break an entire line into tokens, pushing them all into m_buffer.
/// m_buffer may still be empty after a call to LexLine if the line
/// contained no tokens.
/// </summary>
void Lexer::LexLine(std::string_view line)
{
    line = StripComment(line);
    if (IsBlank(line))
    {
        return;
    }

    if (StartsWith(line, "    ") || StartsWith(line, "\t"))
    {
        m_LineNumberBuffer.push(m_lineNumber);
        m_TokenTypeBuffer.push(TokenType::Indent);
        m_TokenTextBuffer.push("");

        line = StripWhitespace(line);

        if (m_expectExpression)
        {
            m_expectExpression = false;
            if (line.back() == '\n')
            {
                line.remove_suffix(1);
            }
            m_LineNumberBuffer.push(m_lineNumber);
            m_TokenTypeBuffer.push(TokenType::Regex);
            m_TokenTextBuffer.push(std::string(line));

            return;
        }

        while (!IsBlank(line))
        {
            line = StripWhitespace(line);
            if (line[0] == '|')
            {
                m_LineNumberBuffer.push(m_lineNumber);
                m_TokenTypeBuffer.push(TokenType::Alternator);
                m_TokenTextBuffer.push("|");
                line.remove_prefix(1);
            }
            else
            {
                std::string text = LexToken(line);
                m_LineNumberBuffer.push(m_lineNumber);
                m_TokenTypeBuffer.push(TokenType::Text);
                m_TokenTextBuffer.push(text);
            }
        }
    }
    else
    {
        line = StripWhitespace(line);
        std::string text = LexToken(line);
        m_LineNumberBuffer.push(m_lineNumber);
        m_TokenTypeBuffer.push(TokenType::Keyword);
        m_TokenTextBuffer.push(text);

        if (text == "expression")
        {
            m_expectExpression = true;
        }

        while (!IsBlank(line))
        {
            line = StripWhitespace(line);
            text = LexToken(line);
            m_LineNumberBuffer.push(m_lineNumber);
            m_TokenTypeBuffer.push(TokenType::Text);
            m_TokenTextBuffer.push(text);
        }
    }
}

/// <summary>
/// Ensure there is at least one token in the buffer.
/// </summary>
void Lexer::FillBuffer()
{
    while (m_LineNumberBuffer.empty() && !m_data.empty())
    {
        std::string_view line = GetLine();
        LexLine(line);
    }

    if (m_LineNumberBuffer.empty())
    {
        m_LineNumberBuffer.push(m_lineNumber);
        m_TokenTypeBuffer.push(TokenType::Eof);
        m_TokenTextBuffer.push("");
    }
}

/// <summary>
/// Determine whether toSearch starts with find.
/// </summary>
/// <param name="toSearch">What to search.</param>
/// <param name="find">What to find.</param>
/// <returns>Whether toSearch starts with find.</returns>
bool StartsWith(const std::string_view& toSearch, const std::string find)
{
    for (size_t i = 0; i < toSearch.size() && i < find.size(); i++)
    {
        if (toSearch[i] != find[i])
        {
            return false;
        }
    }
    return true;
}

/// <summary>
/// Remove the comment from the end of a line, if any.
/// </summary>
/// <param name="line">Line to remove the comment from.</param>
/// <returns>The line, with any comments removed.</returns>
std::string_view StripComment(std::string_view line)
{
    size_t marker = line.find_first_of("#");
    if (marker == std::string_view::npos)
    {
        return line;
    }

    return line.substr(0, marker);
}

/// <summary>
/// Remove any leading whitespace in a line.
/// </summary>
/// <param name="line">The line to strip.</param>
/// <returns>The stripped line.</returns>
std::string_view StripWhitespace(std::string_view line)
{
    size_t start = line.find_first_not_of(whitespace);
    if (start != std::string_view::npos)
    {
        return line.substr(start);
    }

    return line;
}

/// <summary>
/// Check whether a line is empty or consists only of whitespace.
/// </summary>
/// <param name="line">The line to check.</param>
/// <returns>Whether the line is blank.</returns>
bool IsBlank(std::string_view line)
{
    return (line.empty()
            || line.find_first_not_of(whitespace) == std::string_view::npos);
}
