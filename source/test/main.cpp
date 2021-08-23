#include <fstream>
#include <iostream>

#include <lexer/lexer.hpp>

void RunTest()
{
	std::ifstream in("../../../tests/lexer/01-in.txt");
	std::ofstream out("../../../tests/lexer/01-out.txt");
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
		out << lexer.Current() << "\n";
		lexer.Shift();
	}
}

bool CompareOutput()
{
	std::ifstream base("../../../tests/lexer/01-base.txt");
	std::ifstream out("../../../tests/lexer/01-out.txt");

	bool good = true;

	while (base.good() && out.good())
	{
		if (base.get() != out.get())
		{
			good = false;
			break;
		}
	}

	if (base.eof() && out.eof() && good)
	{
		return true;
	}

	return false;
}

int main()
{
	RunTest();
	bool success = CompareOutput();

	if (success)
	{
		return 0;
	}

	std::cout << "Files different\n";
	return 1;
}
