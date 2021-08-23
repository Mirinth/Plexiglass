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

std::string ReadFile(std::string path)
{
	std::ifstream in(path);
	std::string data;

	// https://stackoverflow.com/a/2602060
	in.seekg(0, std::ios::end);
	data.reserve(in.tellg());
	in.seekg(0, std::ios::beg);
	data.assign((std::istreambuf_iterator<char>(in)),
		std::istreambuf_iterator<char>());

	return data;
}

bool RunLexerTest(std::string stem)
{
	std::ofstream out(stem + "-out.txt");
	std::string data = ReadFile(stem + "-in.txt");
	Lexer lexer(data);

	while (lexer.Current().type != TokenType::Eof)
	{
		out << lexer.Current() << "\n";
		lexer.Shift();
	}

	out.close();
	return CompareOutput(stem + "-base.txt", stem + "-out.txt");
}

bool TestLexer()
{
	auto stems = GetTestStems("lexer");

	for (auto& stem : stems)
	{
		std::cout << stem + "-in.txt : ";

		bool pass = RunLexerTest(stem);

		if (pass)
		{
			std::cout << "PASS\n";
		}
		else
		{
			std::cout << "FAIL\n";
			return false;
		}
	}
	
	return true;
}

int main()
{
	bool success = TestLexer();
	
	if (success)
	{
		return 0;
	}

	std::cout << "Files different\n";
	return 1;
}
