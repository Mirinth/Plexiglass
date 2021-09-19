#include <template/template.hpp>

#include <fstream>

void Template(FileNode /*file*/, std::string path)
{
    std::ofstream out(path);

    out << "Template";
}