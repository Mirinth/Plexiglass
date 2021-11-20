#pragma once

#include <filesystem>
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

enum class LexerState
{
    _,
    Initial,
    ExpressionKeyword,
    ExpressionName,
    ExpressionNewline,
    ExpressionIndent,
    ExpressionPattern,
};

std::string ToString(TokenType type, const std::string& text);

class Lexer
{
public:
    Lexer(const std::filesystem::path& path);
    size_t PeekLine() const;
    TokenType PeekToken() const;
    std::string PeekText() const;
    void Shift();

private:
    bool ShiftHelper();

    std::string m_fileContent;
    std::string_view m_view;
    LexerState m_state;
    size_t m_line;
    TokenType m_token;
    std::string m_text;
};
