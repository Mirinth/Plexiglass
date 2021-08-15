#include <fstream>
#include <iostream>

#include "lexer.hpp"

int main()
{
	std::ifstream in("../../../tests/lexer/01-in.txt");
	std::string data;

	// https://stackoverflow.com/a/2602060
	in.seekg(0, std::ios::end);
	data.reserve(in.tellg());
	in.seekg(0, std::ios::beg);
	data.assign((std::istreambuf_iterator<char>(in)),
		std::istreambuf_iterator<char>());

	Lexer lexer(data);

	while (lexer.Current().type != TokenType::Eof)
	{
		std::cout << lexer.Current() << "\n";
		lexer.Shift();
	}

	return 0;
}
