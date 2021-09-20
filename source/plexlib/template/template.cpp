#include <template/template.hpp>

#include <filesystem>
#include <fstream>

void Template(FileNode /*file*/, std::string dir, std::string base)
{
    std::filesystem::remove_all(dir);
    std::filesystem::create_directories(dir);

    std::ofstream out(base + ".hpp");

    out << "Template";
}