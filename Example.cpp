#include <iostream>
#include "ComM.h"


#define ABF Automata::Boolean_Function
#define AND Automata::Boolean_Function::Operation::AND
#define OR Automata::Boolean_Function::Operation::OR
#define NOT Automata::Boolean_Function::Operation::NOT
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

	////create the transition rule set
	//Automata::NFARules  rule_set{};

	//Automata::NFARules::State_Transitions a_transitions = { { zero , one },{ one , one },{ two , one },{ three,three } };
	//rule_set.add(a, a_transitions);

	//Automata::NFARules::State_Transitions b_transitions = { { zero , zero },{ one , two },{ two , three },{ three,three } };
	//rule_set.add(b, b_transitions);



	////create the NFA
	//Automata::NFA nfa{NUMBER_OF_STATES, {zero},{three},rule_set };


	////See if the NFA accepts the following words
	//std::cout << static_cast<int>(nfa.accept({ a,b,a })) << "\n";
	//std::cout << static_cast<int>(nfa.accept({ a,b,a,a,a,b,b,a,a })) << "\n";
	//std::cout << static_cast<int>(nfa.accept({ a,b,b })) << "\n";

	auto f_a = ABF(zero, three, AND);
	auto f_b = ABF(&f_a,NOT);
	auto f_c = ABF(&f_b,one, OR);
	auto f_d = ABF(&f_c,two, AND);

	Automata::r_AFA_Transition_Function transition_function{};

	Automata::Disjunctive_Normal_Form DNF0a{ NUMBER_OF_STATES };
	DNF0a.add_term({ {"0100"s,0,4},{"0000"s,0,4} });
	DNF0a.add_term({ {"0010"s,0,4},{"0000"s,0,4} });
	transition_function.add(zero, a, &DNF0a);
	Automata::Disjunctive_Normal_Form DNF0b{ NUMBER_OF_STATES };
	DNF0b.add_term({ {"0100"s,0,4},{"0000"s,0,4} });
	DNF0b.add_term({ {"0010"s,0,4},{"0000"s,0,4} });
	DNF0b.add_term({ {"0001"s,0,4},{"0000"s,0,4} });
	transition_function.add(zero, b, &DNF0b);

	Automata::Disjunctive_Normal_Form DNF1a{ NUMBER_OF_STATES };
	DNF1a.add_term({ {"0100"s,0,4},{"0100"s,0,4} });
	DNF1a.add_term({ {"0011"s,0,4},{"0011"s,0,4} });
	transition_function.add(one, a, &DNF1a);
	Automata::Disjunctive_Normal_Form DNF1b{ NUMBER_OF_STATES };
	DNF1b.add_term({ {"0111"s,0,4},{"0111"s,0,4} });
	transition_function.add(one, b, &DNF1b);

	Automata::Disjunctive_Normal_Form DNF2a{ NUMBER_OF_STATES };
	DNF2a.add_term({ {"0110"s,0,4},{"0110"s,0,4} });
	DNF2a.add_term({ {"0011"s,0,4},{"0010"s,0,4} });
	DNF2a.add_term({ {"0011"s,0,4},{"0001"s,0,4} });
	transition_function.add(two, a, &DNF2a);
	Automata::Disjunctive_Normal_Form DNF2b{ NUMBER_OF_STATES };
	DNF2b.add_term({ {"0111"s,0,4},{"0111"s,0,4} });
	transition_function.add(two, b, &DNF2b);

	Automata::Disjunctive_Normal_Form DNF3a{ NUMBER_OF_STATES };
	DNF3a.add_term({ {"0110"s,0,4},{"0110"s,0,4} });
	DNF3a.add_term({ {"0101"s,0,4},{"0100"s,0,4} });
	DNF3a.add_term({ {"0011"s,0,4},{"0010"s,0,4} });
	transition_function.add(three, a, &DNF3a);
	Automata::Disjunctive_Normal_Form DNF3b{ NUMBER_OF_STATES };
	DNF3b.set_true();
	transition_function.add(three, b, &DNF3b);


	Automata::r_AFA r_AFA{ NUMBER_OF_STATES,{},&transition_function };

	std::cout << r_AFA.accept({ a,b,a,a,a }) << "\n";//should be true
	std::cout << r_AFA.accept({ a,b,b,b,b,b,b,b,a}) << "\n";//should be false
	std::cout << r_AFA.accept({ a,a,a,a,a}) << "\n";//should be true
	std::cout << r_AFA.accept({ a,a,a,b,b,b,b,b,b,b,a,a }) << "\n";//should be false

	return 0;
}