#include <iostream>
import DFA;


//accepts languages in the form (a+b)* abb (a+b)*
//See "Example DFA.png" for a diagram of this automaton

enum states
{
	zero,
	one,
	two,
	three
};

enum alphabet
{
	a,
	b
};


int main()
{

    //create the transition rule set
	automata::Rules<states, alphabet>  rule_set{};

	rule_set.add({ zero,a }, one);
	rule_set.add({ one,a }, one);
	rule_set.add({ two,a }, one);
	rule_set.add({ three,a }, three);

	rule_set.add({ zero,b }, zero);
	rule_set.add({ one,b }, two);
	rule_set.add({ two,b }, three);
	rule_set.add({ three,b }, three);

	//create the DFA
	automata::DFA<states, alphabet> dfa{ zero,{three},rule_set };


	//See if the DFA accepts the following words
	std::cout << static_cast<int>(dfa.accept({ a,b,a })) << "\n";
	std::cout << static_cast<int>(dfa.accept({ a,b,a,a,a,b,b,a,a })) << "\n";
	std::cout << static_cast<int>(dfa.accept({ a,b,b })) << "\n";
}