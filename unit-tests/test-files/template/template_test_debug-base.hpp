#pragma once

#include <string>
#include <string_view>

enum TokenType
{
    CatToken,
    DogToken,
    PLEXIGLASS_EOF,
    PLEXIGLASS_NO_MATCH_TOKEN,
};

std::string ToString(TokenType type);

class template_test_debug
{
public:
    template_test_debug(std::string path);
    TokenType PeekToken() const;
    std::string PeekText() const;
    void Shift();

private:
    std::string m_input;
    std::string_view m_data;
    TokenType m_type;
    std::string m_text;

    bool ShiftHelper();
};