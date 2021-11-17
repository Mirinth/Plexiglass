#pragma once

#include <queue>
#include <string>
#include <string_view>

enum class TokenType
{
    Alternator,
    Eof,
    Indent,
    Keyword,
    Regex,
    Text,
    Unknown,
};

std::string ToString(TokenType type, const std::string& text);

class Lexer
{
public:
    Lexer(const std::string& path);
    size_t PeekLine() const;
    TokenType PeekToken() const;
    std::string PeekText() const;
    void Shift();

private:
    std::string LexToken(std::string_view& line);
    std::string_view GetLine();
    void LexLine(std::string_view line);
    void FillBuffer();

    std::string m_fileContent;
    std::string_view m_data;
    std::queue<size_t> m_LineNumberBuffer;
    std::queue<TokenType> m_TokenTypeBuffer;
    std::queue<std::string> m_TokenTextBuffer;
    size_t m_lineNumber;
    bool m_expectExpression;
};
