#pragma once

#include <vector>

struct Transition;

class FiniteStateMachine
{
public:
	FiniteStateMachine();
private:
	std::vector<Transition> transitions;
};