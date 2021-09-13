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
