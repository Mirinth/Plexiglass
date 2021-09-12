#include <algorithm>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include <lexer/lexer.hpp>
#include <parser/parser.hpp>

bool IsInputFile(std::filesystem::path file)
{
	if (file.extension() != ".txt")
	{
		return false;
	}

	std::string stem = file.stem().string();
	std::string suffix = "-in";

	if (stem.size() < suffix.size())
	{
		return false;
	}

	return std::equal(suffix.rbegin(), suffix.rend(), stem.rbegin());
}

std::vector<std::string> GetTestStems(std::string directory)
{
	std::vector<std::string> stems;
	std::filesystem::path base = std::filesystem::absolute("../../../tests/" + directory).lexically_normal();

	for (auto& file : std::filesystem::directory_iterator(base))
	{
		if (!IsInputFile(file))
		{
			continue;
		}

		std::string stem = file.path().stem().string();
		size_t location = stem.rfind('-');
		stem.erase(location);
		stems.push_back((base / stem).string());
	}

	std::sort(stems.begin(), stems.end());

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

	while (lexer.Peek().type != TokenType::Eof)
	{
		out << lexer.Peek().ToString() << "\n";
		lexer.Shift();
	}

	out.close();
	return CompareOutput(stem + "-base.txt", stem + "-out.txt");
}

bool RunParserTest(std::string stem)
{
	std::string data = ReadFile(stem + "-in.txt");
	
	try
	{
		Parse(data); // This should always throw for parser tests.

		std::ofstream out(stem + "-out.txt");
		out << "Test failed: No errors" << std::endl;
		return false;
	}
	catch (ParseException exc)
	{
		std::ofstream out(stem + "-out.txt");
		out << exc.what() << std::endl;
		return CompareOutput(stem + "-base.txt", stem + "-out.txt");
	}
}

bool RunTreeTest(std::string stem)
{
	std::string data = ReadFile(stem + "-in.txt");

	try
	{
		FileNode file = Parse(data);

		std::ofstream out(stem + "-out.txt");
		out << file;
		return CompareOutput(stem + "-base.txt", stem + "-out.txt");
	}
	catch (ParseException exc)
	{
		std::ofstream out(stem + "-out.txt");
		out << exc.what() << std::endl;
		return false; // Tree tests should never throw.
	}
}

bool TestGroup(std::string name, std::function<bool(std::string)> test)
{
	auto stems = GetTestStems(name);

	for (auto& stem : stems)
	{
		std::cout << stem + "-in.txt : ";

		bool pass = test(stem);

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
	bool success = TestGroup("lexer", RunLexerTest);
	if (!success)
	{
		return 1;
	}

	success = TestGroup("parser", RunParserTest);
	if (!success)
	{
		return 1;
	}

	success = TestGroup("tree", RunTreeTest);
	if (!success)
	{
		return 1;
	}

	return 0;
}
