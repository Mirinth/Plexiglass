#include "template_test_debug.hpp"

#include <filesystem>
#include <fstream>
#include <regex>
#include <vector>

std::string ReadFile(const std::filesystem::path& path);

struct Rule
{
    /// <summary>
    /// Construct a Rule.
    /// </summary>
    /// <param name="token">The TokenType produced.</param>
    /// <param name="produce">Whether a token is produced at all.</param>
    /// <param name="increment">
    /// How much to change the current line number by.
    /// </param>
    /// <param name="pattern">
    /// A regular expression describing what the rule matches.
    /// </param>
    Rule(TokenType token, bool produce, int increment, const char* pattern)
        : Token(token), Produce(produce), Increment(increment), Pattern(pattern)
    {
    }

    TokenType Token;
    bool Produce;
    int Increment;
    std::regex Pattern;
};

/// <summary>
/// Get the rules to be used by the lexer.
/// </summary>
/// <returns>The rules to be used by the lexer.</returns>
std::vector<Rule> GetRules()
{
    std::vector<Rule> rules;

    rules.emplace_back(CatToken, true, 0, "cat");
    rules.emplace_back(DogToken, true, 0, "dog");
    rules.emplace_back(PLEXIGLASS_EOF, false, 0, "\\s+");

    return rules;
}

/// <summary>
/// Get a human-readable string representation of a token.
/// </summary>
/// <param name="type">The token's type..</param>
/// <param name="text">The token's text.</param>
/// <returns>String representation of the token.</returns>
std::string ToString(TokenType type, const std::string& text)
{
    std::string str;
    switch (type)
    {
    case CatToken:
        str = "CatToken";
        break;
    case DogToken:
        str = "DogToken";
        break;
    case PLEXIGLASS_EOF:
        str = "PLEXIGLASS_EOF";
        break;
    case PLEXIGLASS_NO_MATCH_TOKEN:
        str = "PLEXIGLASS_NO_MATCH_TOKEN";
        break;
    default:
            throw std::exception("Unrecognized token type in ToString()");
    }

    if (!text.empty())
    {
        str += " " + text;
    }

    return str;
}

/// <summary>
/// Construct template_test_debug.
/// </summary>
/// <param name="path">Path to the file to lex.</param>
template_test_debug::template_test_debug(const std::filesystem::path& path)
{
    m_input = ReadFile(path);
    m_data = m_input;
    m_line = 1;
    Shift();
}

/// <summary>
/// Retrieve the line the next token starts on.
/// </summary>
/// <returns>The line the next token starts on.</returns>
size_t template_test_debug::PeekLine() const
{
    return m_line;
}

/// <summary>
/// Retrieve the next TokenType without removing it.
/// </summary>
/// <returns>The next TokenType.</returns>
TokenType template_test_debug::PeekToken() const
{
    return m_type;
}

/// <summary>
/// Retrieve the next token's text without removing it.
/// </summary>
/// <returns>The next token's text.</returns>
std::string template_test_debug::PeekText() const
{
    return m_text;
}

/// <summary>
/// Advance the lexer to the next token.
/// </summary>
void template_test_debug::Shift()
{
    bool success = false;
    while (!success)
    {
        success = ShiftHelper();
    }
}

/// <summary>
/// Helper function for template_test_debug::Shift().
/// </summary>
/// <returns>
/// true if the found token should be used, false if it should be skipped.
/// </returns>
bool template_test_debug::ShiftHelper()
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
            throw std::exception("template_test_debug::Shift(): Length was negative.");
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
        m_line += rules[max_index].Increment;
        return true;
    }
    else if (max_length > 0)
    {
        m_data.remove_prefix(max_length);
        m_line += rules[max_index].Increment;
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

/// <summary>
/// Read the contents of a file in as a string.
/// </summary>
/// <param name="path">Path to the file to read the contents of.</param>
/// <returns>The contents of the file.</returns>
std::string ReadFile(const std::filesystem::path& path)
{
    std::ifstream in(path);
    std::string data;

    auto fileSize = std::filesystem::file_size(path);
    data.reserve(fileSize);
    data.assign(std::istreambuf_iterator<char>(in),
                std::istreambuf_iterator<char>());

    return data;
}

#if 1 // Used to include/exclude driver code. Filled in by templater.

#include <fstream>
#include <iostream>

#include <path.hpp>

/// <summary>
/// Runs the lexer, writing all the tokens it generates to an output file.
/// </summary>
/// <param name="inputPath">Path to file to lex.</param>
/// <param name="outputPath">Path to output file.</param>
void RunLexer(std::string inputPath, std::string outputPath)
{
    lexer lex(inputPath);

    std::ofstream out(outputPath);

    while (lex.PeekToken() != TokenType::PLEXIGLASS_EOF)
    {
        out << ToString(lex.PeekToken(), lex.PeekText()) << "\n";
        lex.Shift();
    }

    out << ToString(lex.PeekToken(), lex.PeekText()) << "\n";
}

/// <summary>
/// Main entry point for lexer driver code.
/// </summary>
/// <param name="argc">Number of command line parameters.</param>
/// <param name="argv">Command line parameters.</param>
/// <returns>
/// 0 if the lexer ran, -1 if command line parameters were bad.
/// </returns>
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
    return 0;
}

#endif
