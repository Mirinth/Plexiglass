#pragma once

#include <string>
#include <string_view>

enum TokenType
{
	CatToken,
	DogToken,
	PLEXIGLASS_NO_MATCH_TOKEN,
};

class L101
{
public:
    L101(std::string path);
    TokenType PeekToken() const;
    std::string PeekText() const;
    void Shift();

private:
    std::string m_input;
    std::string_view m_data;
    TokenType m_type;
    std::string m_text;
};