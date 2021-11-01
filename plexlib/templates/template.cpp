#include "$LEXER_NAME.hpp"

#include <filesystem>
#include <fstream>
#include <regex>
#include <vector>

std::string ReadFile(std::string path);

struct Rule
{
    Rule(TokenType token, bool produce, int increment, const char* pattern)
        : Token(token), Produce(produce), Increment(increment), Pattern(pattern)
    {
    }

    TokenType Token;
    bool Produce;
    int Increment;
    std::regex Pattern;
};

std::vector<Rule> GetRules()
{
    std::vector<Rule> rules;

    $LEXER_RULES

    return rules;
}

std::string ToString(TokenType type)
{
    switch (type)
    {
        $TOKEN_TO_STRING
    }
}

$LEXER_NAME::$LEXER_NAME(std::string path)
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
    bool success = false;
    while (!success)
    {
        success = ShiftHelper();
    }
}

bool $LEXER_NAME::ShiftHelper()
{
    if (m_data.empty())
    {
        m_type = $EOF_TOKEN;
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
            throw std::exception("$LEXER_NAME::Shift(): Length was negative.");
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
        m_type = $INVALID_TOKEN;
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

#if $DEBUG_MODE // Used to include/exclude driver code. Filled in by templater.

#include <fstream>
#include <iostream>

#include <path.hpp>

void RunLexer(std::string inputPath, std::string outputPath)
{
    lexer lex(inputPath);

    std::ofstream out(outputPath);

    while (lex.PeekToken() != TokenType::PLEXIGLASS_EOF)
    {
        out << ToString(lex.PeekToken()) << " " << lex.PeekText() << "\n";
        lex.Shift();
    }

    out << ToString(lex.PeekToken()) << lex.PeekText() << "\n";
}

int main(int argc, char** argv)
{
    argc--; // discard program name
    argv++;

    if (argc != 2)
    {
        std::cout << "Invalid arguments. Call with just input and output "
                     "filenames.\n";
        return -1;
    }

    std::string input = argv[0];
    std::string output = argv[1];

    RunLexer(input, output);
}

#endif
