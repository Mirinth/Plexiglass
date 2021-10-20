#include <fstream>
#include <iostream>

#include <path.hpp>

#include <lexer.hpp>

std::string ReadFile(std::string path);

void RunLexer(std::string inputPath, std::string outputPath)
{
    lexer lex(inputPath);

    std::ofstream out(outputPath);

    while (lex.PeekToken() != TokenType::PLEXIGLASS_EOF)
    {
        out << ToString(lex.PeekToken()) << " " << lex.PeekText() << "\n";
        lex.Shift();
    }

    out << ToString(lex.PeekToken()) << " " << lex.PeekText() << "\n";
}

int main()
{
    std::string basePath = base_path;
    std::string inputPath = basePath + "input.txt";
    std::string outputPath = basePath + "out.txt";

    RunLexer(inputPath, outputPath);
    
    std::string base = ReadFile(basePath);
    std::string out = ReadFile(outputPath);

    if (base == out)
    {
        std::cout << "Pass\n";
        return 0;
    }
    else
    {
        std::cout << "Fail\n";
        return 1;
    }
}
