#include <error.hpp>

#include <sstream>

#include <lexer/token.hpp>

/// <summary>
/// Initialize the exception.
/// </summary>
/// <param name="msg">The exception's message.</param>
PlexiException::PlexiException(const char* msg)
	: std::exception(msg)
{}

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
/// Generate an error message and stop parsing.
/// </summary>
/// <param name="line">The line the error occurred on.</param>
/// <param name="message">The message to display.</param>
void Error(size_t line, std::string message)
{
	std::stringstream out;
	out << "Error on line " << line
		<< ": " << message;
	throw PlexiException(out.str().c_str());
}

/// <summary>
/// Generate an error message and stop parsing.
/// </summary>
/// <param name="expected">What was expected.</param>
/// <param name="tok">What was found.</param>
void Error(std::string expected, const Token tok)
{
	std::stringstream out;
	out << "Error on line " << tok.line
		<< ": Expected " << expected
		<< " found " << tok.ToString();
	throw PlexiException(out.str().c_str());
}
