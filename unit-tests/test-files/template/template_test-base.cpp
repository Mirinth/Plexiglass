#include "template_test.hpp"

#include <filesystem>
#include <fstream>
#include <regex>
#include <vector>

std::string ReadFile(std::string path);

struct Rule
{
    bool Nothing;
    bool Produce;
    int Line;
    std::regex Pattern;
};

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