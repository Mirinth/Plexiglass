#include <iostream>

#include <lexer.hpp>

#include <path.hpp>

int main()
{
    std::string basePath = base_path;
    std::string lexerPath = basePath + "lexer.txt";
    lexer lex(lexerPath);



    std::cout << "Integration test\n";
    return 0;
}
