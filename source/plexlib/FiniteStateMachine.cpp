#include "FiniteStateMachine.hpp"

struct Transition
{
	int id;
	char read;
	bool isLambda;
	int target;
};

FiniteStateMachine::FiniteStateMachine()
{
}
