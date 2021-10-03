#include <filesystem>
#include <iostream>
#include <string>

void PrintUsage()
{
    std::cout
        << "Usage:\n"
        << "    plexiglass filename\n"
        << "\n"
        << "  filename: Name of the input file.\n"
        << "\n"
        << "Note: Output file and lexer name are based on input file name.\n";
}

int main(int argc, const char** argv)
{
    if (argc != 2)
    {
        PrintUsage();
        return 1;
    }

    argc--; // discard executable name
    argv++;

    std::string param = argv[0];

    if (param == "-h")
    {
        PrintUsage();
        return 1;
    }

    std::filesystem::path source = param;
    std::filesystem::path header = source.replace_extension(".hpp");
    std::filesystem::path code = source.replace_extension(".cpp");
    std::string lexerName = source.filename().stem().string();
    
    std::cout << "Filename: " << source << "\n";
    std::cout << "Lexer name: " << lexerName << "\n";
    std::cout << "Header name: " << header << "\n";
    std::cout << "Code name: " << code << "\n";

    return 0;
}
