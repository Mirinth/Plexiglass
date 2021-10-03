#include <iostream>

void PrintUsage()
{
    std::cout
        << "Usage:\n"
        << "plexiglass filename\n"
        << "\n"
        << "  filename: Name of the input file.\n"
        << "\n"
        << "Note: Output file and lexer name are based on input file name.\n";
}

int main()
{
    PrintUsage();
	return 0;
}
