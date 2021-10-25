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

template_test::template_test(std::string path)
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
    bool success = false;
    while (!success)
    {
        success = ShiftHelper();
    }
}

bool template_test::ShiftHelper()
{
    if (m_data.empty())
    {
        m_type = PLEXIGLASS_EOF;
        m_text = "";
        return true;
    }

    using vmatch = std::match_results<std::string_view::const_iterator>;
    static std::vector<Rule> rules = GetRules();

    size_t max_index = 0;
    size_t max_length = 0;
    std::string max_string;

    for (size_t index = 0; index < rules.size(); index++)
    {
        Rule rule = rules[index];
        vmatch m;
        bool matched =
            std::regex_search(m_data.begin(), m_data.end(), m, rule.Pattern);
        if (!matched || m.position() != 0)
        {
            continue;
        }

        // Ensure following cast is safe
        if (m.length() < 0)
        {
            throw std::exception("template_test::Shift(): Length was negative.");
        }
        size_t length = static_cast<size_t>(std::abs(m.length()));

        if (length > max_length)
        {
            max_length = length;
            max_index = index;
            max_string = m.str();
        }
    }

    if (max_length > 0 && rules[max_index].Produce)
    {
        m_type = rules[max_index].Token;
        m_text = m_data.substr(0, max_length);
        m_data.remove_prefix(max_length);
        return true;
    }
    else if (max_length > 0)
    {
        m_data.remove_prefix(max_length);
        return false;
    }
    else
    {
        m_type = PLEXIGLASS_NO_MATCH_TOKEN;
        m_text = "";
        m_data.remove_prefix(1);
        return true;
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