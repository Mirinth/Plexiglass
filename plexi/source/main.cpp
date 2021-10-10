#include <iostream>
#include <string>
#include <vector>

#include <plexlib.hpp>

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
