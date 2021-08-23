#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <lexer/lexer.hpp>

std::vector<std::string> GetTestStems(std::string directory)
{
	std::vector<std::string> stems;
	std::filesystem::path base = std::filesystem::absolute("../../../tests/" + directory).lexically_normal();

	for (auto& file : std::filesystem::directory_iterator(base))
	{
		std::string stem = file.path().stem().string();
		size_t location = stem.rfind('-');
		stem.erase(location);
		stems.push_back((base / stem).string());
	}

	return stems;
}

bool CompareOutput(std::string basePath, std::string outPath)
{
	std::ifstream base(basePath);
	std::ifstream out(outPath);

	while (base.good() && out.good())
	{
		if (base.get() != out.get())
		{
			return false;
		}
	}

	if (base.eof() && out.eof())
	{
		return true;
	}

	return false;
}

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



int main()
{
	RunTest();
	bool success = CompareOutput("../../../tests/lexer/01-base.txt", "../../../tests/lexer/01-out.txt");

	if (success)
	{
		return 0;
	}

	std::cout << "Files different\n";
	return 1;
}
