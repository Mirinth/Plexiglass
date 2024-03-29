#include <iostream>
#include <string>
#include <vector>

#include <plexlib.hpp>

/// <summary>
/// The entry point for Plexiglass.
/// </summary>
/// <param name="argc">Number of command line parameters.</param>
/// <param name="argv">Command line parameters.</param>
/// <returns>
/// One of the constants listed at the top of plexlib.hpp.
/// </returns>
int main(int argc, const char** argv)
{
    argc--; // discard program name
    argv++;

    std::vector<std::string> args;

    for (int i = 0; i < argc; i++)
    {
        args.push_back(argv[i]);
    }

    PlexMain(args, std::cout, std::cerr);
}
