#include <plexlib.hpp>

#include <filesystem>
#include <string>

#include <analyzer/analyzer.hpp>
#include <error.hpp>
#include <parser/parser.hpp>
#include <parser/tree.hpp>
#include <template/template.hpp>
#include <utils.hpp>

/// <summary>
/// Write the Plexiglass usage message to out.
/// </summary>
/// <param name="out">Output stream to write the usage message to.</param>
void PrintUsage(std::ostream& out)
{
    out << "Usage:\n"
        << "    plexiglass [--debug] filename\n"
        << "\n"
        << "  --debug: Generate a lexer with a debug driver.\n"
        << "\n"
        << "  filename: Name of the input file.\n"
        << "\n"
        << "Note: Output file and lexer name are based on input file name.\n";
}

/// <summary>
/// Check whether a lexer name is valid.
/// </summary>
/// <param name="name">The name to check.</param>
/// <returns>Whether the name is valid.</returns>
bool IsValidLexerName(const std::string& name)
{
    if (name.size() == 0)
    {
        return false;
    }

    if (name[0] != '_' && !isalpha(name[0]))
    {
        return false;
    }

    for (const auto& c : name)
    {
        if (c != '_' && !isalnum(c))
        {
            return false;
        }
    }

    return true;
}

/// <summary>
/// Parse the program arguments into variables.
/// </summary>
/// <param name="args">Arguments to parse.</param>
/// <param name="path">Initialized to the lexer file's path.</param>
/// <param name="help">Initialized to whether help was requested.</param>
/// <param name="debug">
/// Initialized to whether debug mode was requested.
/// </param>
/// <returns>Whether the command line arguments were valid.</returns>
bool ParseArgs(const std::vector<std::string>& args,
               std::string& path,
               bool& help,
               bool& debug)
{
    path = "";
    help = false;
    debug = false;
    bool good = true;

    for (const auto& arg : args)
    {
        if (arg == "-h" || arg == "--help")
        {
            if (help)
            {
                good = false;
            }
            help = true;
        }
        else if (arg == "-d" || arg == "--debug")
        {
            if (debug)
            {
                good = false;
            }
            debug = true;
        }
        else
        {
            if (path.size() > 0)
            {
                good = false;
            }
            path = arg;
        }
    }

    return good;
}

/// <summary>
/// Runs Plexiglass and generates a lexer.
/// </summary>
/// <param name="args">Command line arguments.</param>
/// <param name="out">Where to write output messages. Can be std::cout.</param>
/// <param name="err">Where to write error messages. Can be std::cerr.</param>
/// <returns></returns>
int PlexMain(std::vector<std::string>& args,
             std::ostream& out,
             std::ostream& err)
{
    std::string lexerPath;
    bool help, debug;

    bool good = ParseArgs(args, lexerPath, help, debug);

    if (!good)
    {
        PrintUsage(out);
        return bad_usage;
    }

    if (help)
    {
        PrintUsage(out);
        return help_invoked;
    }

    if (lexerPath.size() == 0)
    {
        PrintUsage(out);
        return bad_usage;
    }

    std::filesystem::path source = lexerPath, header = lexerPath,
                          code = lexerPath;
    header.replace_extension(".hpp");
    code.replace_extension(".cpp");
    std::string lexerName = source.filename().stem().string();

    if (!IsValidLexerName(lexerName))
    {
        err << "File generates lexer named `" << lexerName
            << "` which is not a valid C++ identifier. Rename the file to be "
               "a valid C++ identifier.\n";
        return bad_lexer_name;
    }

    try
    {
        FileNode file = Parse(source.string());
        Analyze(file);
        Template(file, lexerName, header.string(), code.string(), debug);
        return success;
    }
    catch (const PlexiException& exc)
    {
        err << "Generation failed: " << exc.what() << std::endl;
        return bad_lexer;
    }
    catch (const std::filesystem::filesystem_error& exc)
    {
        // path can be inserted directly, but it prints an escaped string,
        // which might mislead users into thinking the problem is redundant
        // backslashes.
        auto path = std::filesystem::absolute(exc.path1());
        err << "Unable to read file " << path.string() << std::endl;
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
