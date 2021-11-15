#include <error.hpp>

#include <sstream>

#include <lexer/lexer.hpp>

/// <summary>
/// Initialize the exception.
/// </summary>
/// <param name="msg">The exception's message.</param>
PlexiException::PlexiException(const char* msg) : std::exception(msg) {}

/// <summary>
/// Generate a duplicate action error and stop parsing.
/// </summary>
/// <param name="originalLine">Line action was originally used on.</param>
/// <param name="original">Line duplicate action is on.</param>
/// <param name="name">Name of the action.</param>
void DuplicateActionError(size_t originalLine,
                          size_t duplicateLine,
                          std::string name)
{
    std::stringstream out;
    out << "Error on line " << duplicateLine << ": "
        << "`" << name << "` already used in rule on line " << originalLine;
    throw PlexiException(out.str().c_str());
}

/// <summary>
/// Generate a duplicate name error message and stop parsing.
/// </summary>
/// <param name="line">Line to report the error on.</param>
/// <param name="original">Line where name was originally defined.</param>
/// <param name="name">Name that was duplicated.</param>
void DuplicateNameError(size_t line, size_t original, std::string name)
{
    std::stringstream out;
    out << "Error on line " << line << ": "
        << "'" << name << "' already defined on line " << original;
    throw PlexiException(out.str().c_str());
}

/// <summary>
/// Generate a missing name error message and stop parsing.
/// </summary>
/// <param name="line">Line the name was referenced on.</param>
/// <param name="name">Name that was missing.</param>
void MissingNameError(size_t line, std::string name)
{
    std::stringstream out;
    out << "Error on line " << line << ": "
        << "Undefined name '" << name << "'";
    throw PlexiException(out.str().c_str());
}

/// <summary>
/// Generate an error message and stop parsing.
/// </summary>
/// <param name="line">The line the error occurred on.</param>
/// <param name="message">The message to display.</param>
void Error(size_t line, std::string message)
{
    std::stringstream out;
    out << "Error on line " << line << ": " << message;
    throw PlexiException(out.str().c_str());
}

/// <summary>
/// Generate an error message and stop parsing.
/// </summary>
/// <param name="expected">What was expected.</param>
/// <param name="line">Line number the problem occurred on.</param>
/// <param name="type">TokenType actually found.</param>
/// <param name="text">Text of token actually found.</param>
void Error(std::string expected, size_t line, TokenType type, std::string text)
{
    std::stringstream out;
    out << "Error on line " << line << ": "
        << "Expected " << expected << " found " << ToString(type, text);
    throw PlexiException(out.str().c_str());
}
