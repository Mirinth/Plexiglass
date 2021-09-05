#include <lexer/rules.hpp>

#include <map>

size_t NoMatch = 0;

std::vector<Rule> Rules = {
	{ Literal("\n"),              TokenType::Newline },
	{ Whitespace,                 TokenType::Retry },

	{ Literal("expression"),      TokenType::KwdExpression },
	{ Regex,                      TokenType::Regex },

	{ Literal("pattern"),         TokenType::KwdPattern },
	{ Literal("|"),               TokenType::Alternator },
	
	{ Literal("rule"),            TokenType::KwdRule },
	{ Literal("produce-nothing"), TokenType::ActProduceNil },
	{ Literal("produce"),         TokenType::ActProduce },
	{ Literal("rewind"),          TokenType::ActRewind },
	{ Literal("transition"),      TokenType::ActTransition },
	
	{ Literal("++line"),          TokenType::ActInc },
	{ Literal("line++"),          TokenType::ActInc },
	{ Literal("--line"),          TokenType::ActDec },
	{ Literal("line--"),          TokenType::ActDec },

	{ Identifier,                 TokenType::Identifier },

	{ Error,                      TokenType::Unknown },
};

size_t Whitespace(std::string_view data)
{
	if (data[0] != ' ' && data[0] != '\t')
	{
		return NoMatch;
	}

	size_t size = data.find_first_not_of(" \t");
	return size == std::string_view::npos ? data.size() : size;
}

size_t Identifier(std::string_view data)
{
	size_t size = data.find_first_of(" \t\r\n");
	return size == std::string_view::npos ? data.size() : size;
}

size_t Regex(std::string_view data)
{
	if (data[0] != '^')
	{
		return NoMatch;
	}

	size_t size = data.find_first_of("\r\n");
	return size == std::string_view::npos ? data.size() : size;
}


size_t Error(std::string_view /*data*/)
{
	return 1;
}

bool StartsWith(const std::string_view& toSearch, const std::string find)
{
	for (size_t i = 0; i < toSearch.size() && i < find.size(); i++)
	{
		if (toSearch[i] != find[i])
		{
			return false;
		}
	}
	return true;
}

Matcher Literal(std::string value)
{
	return [value](std::string_view data)
	{
		if (!StartsWith(data, value))
		{
			return NoMatch;
		}

		return value.size();
	};
}
