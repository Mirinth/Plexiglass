#include <fstream>
#include <iostream>

#include <lexer.hpp>

#include <path.hpp>

int main()
{
    std::string basePath = base_path;
    std::string inputPath = basePath + "input.txt";
    std::string outputPath = basePath + "out.txt";
    lexer lex(inputPath);

    std::ofstream out(outputPath);

    while (lex.PeekToken() != TokenType::PLEXIGLASS_EOF)
    {
        out << ToString(lex.PeekToken()) << " " << lex.PeekText() << "\n";
        lex.Shift();
    }

    out << ToString(lex.PeekToken()) << " " << lex.PeekText() << "\n";

    return 0;
}
