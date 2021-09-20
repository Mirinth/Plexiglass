#include "$LEXER_NAME.hpp"

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

$LEXER_NAME::$LEXER_NAME(istream& input)
{
    m_input = ReadFile(path);
    m_data = m_input;
    Shift();
}

TokenType $LEXER_NAME::PeekToken() const
{
    return m_type;
}

std::string $LEXER_NAME::PeekText() const
{
    return m_text;
}

void $LEXER_NAME::Shift()
{
    static std::vector<Rule> rules = {$LEXER_RULES
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
        m_type = $INVALID_TOKEN;
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