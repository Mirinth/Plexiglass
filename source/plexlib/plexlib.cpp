#include <plexlib.hpp>

#include <filesystem>
#include <string>

#include <analyzer/analyzer.hpp>
#include <error.hpp>
#include <parser/parser.hpp>
#include <parser/tree.hpp>
#include <template/template.hpp>
#include <utils.hpp>

void PrintUsage(std::ostream& out)
{
    out << "Usage:\n"
        << "    plexiglass filename\n"
        << "\n"
        << "  filename: Name of the input file.\n"
        << "\n"
        << "Note: Output file and lexer name are based on input file name.\n";
}

int PlexMain(std::vector<std::string>& args,
             std::ostream& out,
             std::ostream& err)
{
    if (args.size() != 1)
    {
        PrintUsage(out);
        return bad_usage;
    }

    std::string param = args[0];

    if (param == "-h")
    {
        PrintUsage(out);
        return help_invoked;
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
        return success;
    }
    catch (const PlexiException& exc)
    {
        err << "Generation failed: " << exc.what() << std::endl;
        return bad_lexer;
    }
    catch (const std::filesystem::filesystem_error&)
    {
        // source can be inserted directly, but it prints an escaped string,
        // which might mislead users into thinking the problem is redundant
        // backslashes.
        err << "Unable to read file " << source.string() << std::endl;
        return unreadable_file;
    }
    catch (const std::exception& exc)
    {
        err << "An unhandled exception occurred: " << exc.what() << std::endl;
        return unknown_fault;
    }
    catch (...)
    {
        err << "An unrecognized exception was thrown.\n";
        return unknown_fault;
    }
}
