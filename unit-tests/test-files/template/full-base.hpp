#pragma once

#include <filesystem>
#include <string>
#include <string_view>

enum class LexerState;

enum class TokenType
{
    PLEXIGLASS_EOF,
    PLEXIGLASS_NO_MATCH_TOKEN,
    secondToken,
};

std::string ToString(TokenType type, const std::string& text);

class full
{
public:
    full(const std::filesystem::path& path);
    size_t PeekLine() const;
    TokenType PeekToken() const;
    std::string PeekText() const;
    void Shift();

private:
    std::string m_reference;
    std::string_view m_view;
    LexerState m_state;
    size_t m_line;
    TokenType m_type;
    std::string m_text;

    bool ShiftHelper();
};