#pragma once

#include <queue>
#include <string>
#include <string_view>

#include <lexer/token.hpp>

class Lexer
{
public:
    Lexer(std::string_view data);
    size_t PeekLine() const;
    TokenType PeekToken() const;
    std::string PeekText() const;
    void Shift();

private:
    std::string LexToken(std::string_view& line);
    std::string_view GetLine();
    void LexLine(std::string_view line);
    void FillBuffer();

    std::string_view m_data;
    std::queue<size_t> m_LineNumberBuffer;
    std::queue<TokenType> m_TokenTypeBuffer;
    std::queue<std::string> m_TokenTextBuffer;
    size_t m_lineNumber;
    bool m_expectExpression;
};
