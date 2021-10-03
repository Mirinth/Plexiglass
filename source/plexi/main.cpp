#include <filesystem>
#include <iostream>
#include <string>

#include <analyzer/analyzer.hpp>
#include <error.hpp>
#include <parser/parser.hpp>
#include <parser/tree.hpp>
#include <template/template.hpp>
#include <utils.hpp>

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
    
    try
    {
        std::string data = ReadFile(source.string());
        FileNode file = Parse(data);
        Analyze(file);
        Template(file, lexerName, header.string(), code.string());
        return 0;
    }
    catch (const PlexiException& exc)
    {
        std::cerr << "Generation failed: " << exc.what() << std::endl;
        return 2;
    }
    catch (const std::filesystem::filesystem_error&)
    {
        // source can be inserted directly, but it prints an escaped string,
        // which might mislead users into thinking the problem is redundant
        // backslashes.
        std::cerr << "Unable to read file " << source.string() << std::endl;
        return 3;
    }
    catch (const std::exception& exc)
    {
        std::cerr << "An unhandled exception occurred: " << exc.what()
                  << std::endl;
    }
    catch (...)
    {
        std::cerr << "An unrecognized exception was thrown.\n";
    }
}
