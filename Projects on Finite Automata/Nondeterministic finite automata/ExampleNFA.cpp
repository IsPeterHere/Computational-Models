#include <iostream>
#include <NFAutomata.h>


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
	automata::NFARules<states, alphabet>  rule_set{};

	automata::NFARules<states, alphabet>::State_Transitions a_transitions = { { zero , one },{ one , one },{ two , one },{ three,three } };
	rule_set.add(a, a_transitions);

	automata::NFARules<states, alphabet>::State_Transitions b_transitions = { { zero , zero },{ one , two },{ two , three },{ three,three } };
	rule_set.add(b, b_transitions);



	//create the NFA
    automata::NFA<states, alphabet, NUMBER_OF_STATES> nfa{ {zero},{three},rule_set };


	//See if the NFA accepts the following words
	std::cout << static_cast<int>(nfa.accept({ a,b,a })) << "\n";
	std::cout << static_cast<int>(nfa.accept({ a,b,a,a,a,b,b,a,a })) << "\n";
	std::cout << static_cast<int>(nfa.accept({ a,b,b })) << "\n";

}