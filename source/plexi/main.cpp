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

    std::string filename = argv[0];

    if (filename == "-h")
    {
        PrintUsage();
        return 1;
    }

    std::cout << "Plexiglass is running\n";

	return 0;
}
