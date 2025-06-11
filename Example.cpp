#include <iostream>
#include "ComM.h"


//accepts languages in the form (a+b)* abb(a+b)*
//See "Example DFA.png" for a diagram of this automaton

enum states
{
	zero,
	one,
	two,
	three,
	NUMBER_OF_STATES
};

enum alphabet
{
	a,
	b
};


int main()
{

	//create the transition rule set
	Automata::NFARules  rule_set{};

	Automata::NFARules::State_Transitions a_transitions = { { zero , one },{ one , one },{ two , one },{ three,three } };
	rule_set.add(a, a_transitions);

	Automata::NFARules::State_Transitions b_transitions = { { zero , zero },{ one , two },{ two , three },{ three,three } };
	rule_set.add(b, b_transitions);



	//create the NFA
	Automata::NFA nfa{NUMBER_OF_STATES, {zero},{three},rule_set };


	//See if the NFA accepts the following words
	std::cout << static_cast<int>(nfa.accept({ a,b,a })) << "\n";
	std::cout << static_cast<int>(nfa.accept({ a,b,a,a,a,b,b,a,a })) << "\n";
	std::cout << static_cast<int>(nfa.accept({ a,b,b })) << "\n";

}