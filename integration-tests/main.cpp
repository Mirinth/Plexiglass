#include <filesystem>
#include <fstream>
#include <iostream>

#include <lexer.hpp>

std::string ReadFile(const std::filesystem::path& path);

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
        out << lex.PeekLine() << ": "
            << ToString(lex.PeekToken(), lex.PeekText()) << "\n";
        lex.Shift();
    }

    out << lex.PeekLine() << ": " << ToString(lex.PeekToken(), lex.PeekText())
        << "\n";
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

    if (argc != 3)
    {
        std::cout << "Invalid arguments. Call with just input and output "
                     "filenames and base filename.\n";
        return -1;
    }

    std::string input = argv[0];
    std::string out = argv[1];
    std::string base = argv[2];

    RunLexer(input, out);

    std::string baseContent = ReadFile(base);
    std::string outContent = ReadFile(out);

    if (baseContent == outContent)
    {
        std::cout << "Pass\n";
        return 0;
    }
    else
    {
        std::cout << "Fail\n" << base << " != " << out << "\n";
        return 1;
    }
}
