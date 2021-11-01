#include <utils.hpp>

#include <filesystem>
#include <fstream>

/// <summary>
/// Read an entire file into a string.
/// </summary>
/// <param name="path">Path to the file.</param>
/// <returns>The file's contents.</returns>
std::string ReadFile(std::string path)
{
    std::ifstream in(path);
    std::string data;

    auto fileSize = std::filesystem::file_size(path);
    data.reserve(fileSize);
    data.assign(std::istreambuf_iterator<char>(in),
                std::istreambuf_iterator<char>());

    return data;
}

/// <summary>
/// String replace function.
/// </summary>
/// <param name="subject">String to replace.</param>
/// <param name="find">What to replace.</param>
/// <param name="replace">What to replace it with.</param>
void Replace(std::string& subject,
             const std::string& find,
             const std::string& replace)
{
    // https://stackoverflow.com/a/3418285
    // https://creativecommons.org/licenses/by-sa/3.0/
    if (find.empty())
    {
        return;
    }

    size_t start_pos = 0;
    while ((start_pos = subject.find(find, start_pos)) != std::string::npos)
    {
        subject.replace(start_pos, find.length(), replace);
        start_pos += replace.length(); // In case 'replace' contains 'find',
                                       // like replacing 'x' with 'yx'
    }
}
