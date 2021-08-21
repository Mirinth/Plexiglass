#include <lexer/rules.hpp>

size_t Newline(std::string_view data, State current, State& next, TokenType& type, std::string& text)
{
	if (data[0] != '\n')
	{
		return 0;
	}

	next = current;
	type = TokenType::Newline;
	text = "\\n";
	return 1;
}
