#include "template_test.hpp"

#include <filesystem>
#include <fstream>
#include <regex>
#include <vector>

std::string ReadFile(std::string path);

struct Rule
{
    Rule(TokenType token, bool produce, int increment, const char* pattern)
        : Token(token), Produce(produce), Increment(increment), Pattern(pattern)
    {}

    TokenType Token;
    bool Produce;
    int Increment;
    std::regex Pattern;
};

std::vector<Rule> GetRules()
{
    std::vector<Rule> rules;

    rules.emplace_back(CatToken, true, 0, "cat");
    rules.emplace_back(DogToken, true, 0, "dog");
    rules.emplace_back(PLEXIGLASS_EOF, false, 0, "\\s+");

    return rules;
}

std::string ToString(TokenType type)
{
    switch (type)
    {
        case CatToken:
            return "CatToken";
        case DogToken:
            return "DogToken";
        case PLEXIGLASS_EOF:
            return "PLEXIGLASS_EOF";
        case PLEXIGLASS_NO_MATCH_TOKEN:
            return "PLEXIGLASS_NO_MATCH_TOKEN";
        default:
            throw std::exception("Unknown token");
    }
}

template_test::template_test(istream& input)
{
    m_input = ReadFile(path);
    m_data = m_input;
    Shift();
}

TokenType template_test::PeekToken() const
{
    return m_type;
}

std::string template_test::PeekText() const
{
    return m_text;
}

void template_test::Shift()
{
    static std::vector<Rule> rules = {
		{ true, CatToken, 0, "cat"};
		{ true, DogToken, 0, "dog"};
		{ false, PLEXIGLASS_NO_MATCH_TOKEN, 0, "\\s+"};
    };

    unsigned int max_index = 0;
    unsigned int max_length = 0;
    std::string max_string;

    for (size_t index = 0; index < rules.length(); index++)
    {
        auto& [type, regex] = rules[index];
        std::match_results m;
        bool matched = std::regex_search(m_data.begin(), m_data.end(), m, regex);
        if (!matched)
        {
            continue;
        }

        if (m.length() > max_length)
        {
            max_length = m.length();
            max_index = index;
            max_string = m.str();
        }
    }

    if (max_length > 0)
    {
        m_type = rules[max_index].first();
        m_text = m_data.substr(0, max_length);
        m_data.remove_prefix(max_length);
    }
    else
    {
        m_type = PLEXIGLASS_NO_MATCH_TOKEN;
        m_text = "";
        m_data.remove_prefix(1);
    }
}

std::string ReadFile(std::string path)
{
    std::ifstream in(path);
    std::string data;

    auto fileSize = std::filesystem::file_size(path);
    data.reserve(fileSize);
    data.assign(std::istreambuf_iterator<char>(in),
                std::istreambuf_iterator<char>());

    return data;
}