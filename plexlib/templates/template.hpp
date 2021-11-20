#pragma once

#include <filesystem>
#include <string>
#include <string_view>

enum TokenType
{
    $TOKEN_NAMES
};

std::string ToString(TokenType type, const std::string& text);

class $LEXER_NAME
{
public:
    $LEXER_NAME(const std::filesystem::path& path);
    size_t PeekLine() const;
    TokenType PeekToken() const;
    std::string PeekText() const;
    void Shift();

private:
    std::string m_input;
    std::string_view m_data;
    size_t m_line;
    TokenType m_type;
    std::string m_text;

    bool ShiftHelper();
};