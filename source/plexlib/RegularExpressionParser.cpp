#include "RegularExpressionParser.hpp"

#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

enum class operation
{
	lparen,
	rparen,
	alternation,
	concatenation,
	empty,
};

typedef int fsm;

void collapse(std::vector<operation>& tokenStack, std::vector<fsm>& fsmStack);
fsm concatenate(fsm lhs, fsm rhs);
fsm alternate(fsm lhs, fsm rhs);
fsm lambda();
fsm kleene(fsm m);
fsm symbol(char c);
bool isLambda(fsm m);


void parseRegularExpression(std::string_view expression)
{
	std::vector<operation> opStack;
	std::vector<fsm> fsmStack = { lambda() };

	while (!expression.empty())
	{
		char next = expression[0];
		expression.remove_prefix(1);

		if (next == '*')
		{
			fsm s = fsmStack.back();
			fsmStack.pop_back();

			if (isLambda(s))
			{
				std::cout << "Error: * not allowed at beginning of expression.\n";
				return;
			}

			fsm kleened = kleene(s);
			fsmStack.push_back(kleened);
		}

		else if (next == ')')
		{
			collapse(opStack, fsmStack);
			if (opStack.empty() || opStack.back() != operation::lparen)
			{
				std::cout << "Error: ) without matching (\n";
				return;
			}
			if (isLambda(fsmStack.back()))
			{
				std::cout << "Error: ) not allowed after (\n";
				return;
			}
		}

		else if (next == '(')
		{
			fsmStack.push_back(lambda());
			fsmStack.push_back(lambda());
			opStack.push_back(operation::concatenation);
			opStack.push_back(operation::lparen);
		}

		else if (next == '|')
		{
			collapse(opStack, fsmStack);
			fsmStack.push_back(lambda());
			opStack.push_back(operation::alternation);
		}

		else
		{
			fsmStack.push_back(symbol(next));
			opStack.push_back(operation::concatenation);
		}
	}

	collapse(opStack, fsmStack);
}

void collapse(std::vector<operation>& opStack, std::vector<fsm>& fsmStack)
{
	while (!opStack.empty() && opStack.back() != operation::lparen)
	{
		fsm rhs = fsmStack.back();
		fsmStack.pop_back();
		fsm lhs = fsmStack.back();
		fsmStack.pop_back();
		operation op = opStack.back();
		opStack.pop_back();

		if (op == operation::concatenation)
		{
			fsmStack.push_back(concatenate(lhs, rhs));
		}

		else if (op == operation::alternation)
		{
			fsmStack.push_back(alternate(lhs, rhs));
		}

		else
		{
			throw "Unrecognized operation";
		}
	}
}

fsm concatenate(fsm /*lhs*/, fsm /*rhs*/)
{
	return 0;
}

fsm alternate(fsm /*lhs*/, fsm /*rhs*/)
{
	return 0;
}

fsm symbol(char /*c*/)
{
	return 0;
}

fsm lambda()
{
	return 7;
}

fsm kleene(fsm /*m*/)
{
	return 0;
}

bool isLambda(fsm m)
{
	return (m == 7);
}
