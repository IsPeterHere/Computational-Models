#include"ComM.h"


bool Automata::Disjunctive_Normal_Term::eval(boost::dynamic_bitset<> inputs) const
{
    if (((inputs &= alpha) ^= beta).none())
        return 1;
    return 0;
}



void  Automata::Disjunctive_Normal_Form::add_term(Disjunctive_Normal_Term term)
{
    assert(term.alpha.size() == size && term.beta.size() == size && "Terms definitions not correct size");
    assert(term.alpha.at(size-1) == 0 && term.beta.at(size - 1) == 0 && "NO term can depend on S (( r-AFA are a variation of s-AFA ))");
    terms.push_back(term);
}

void Automata::Disjunctive_Normal_Form::set_true()
{
    terms.clear();
    terms.push_back({ boost::dynamic_bitset(size), boost::dynamic_bitset(size) });
}

void Automata::Disjunctive_Normal_Form::set_false()
{
    terms.clear();
    Disjunctive_Normal_Term DNT{ boost::dynamic_bitset(size), boost::dynamic_bitset(size) };
    DNT.beta.flip();
    terms.push_back(DNT);
}

bool Automata::Disjunctive_Normal_Form::eval(boost::dynamic_bitset<>* inputs)
{
    for (Disjunctive_Normal_Term& term : terms)
        if (term.eval(*inputs))
            return 1;
    return 0;
}





Automata::Boolean_Function::Boolean_Function(states_T term, Operation operation) : type(Type::STATE), operation(operation), state1(term)
{
    if (operation != Operation::NOT)
        throw std::runtime_error("Boolean function with single term must have operation::NOT");
}
Automata::Boolean_Function::Boolean_Function(Boolean_Function* term, Operation operation) : type(Type::FUNC), operation(operation), func1(term)
{
    if (operation != Operation::NOT)
        throw std::runtime_error("Boolean function with single term must have operation::NOT");
}

Automata::Disjunctive_Normal_Form Automata::Boolean_Function::convert_to_disjunctive_normal_form()
{
    return {0};
}



void Automata::r_AFA_Transition_Function::add(states_T state, alphabet_T letter, Disjunctive_Normal_Form* disjuctive_form)
{
    letter_state_pair pair{ letter,state };
    map[pair] = disjuctive_form;
}

Automata::Disjunctive_Normal_Form* Automata::r_AFA_Transition_Function::operator[](letter_state_pair& key)
{
    auto search{ map.find(key) };
    if (search != map.end())
        return (*search).second;
    throw std::runtime_error("Transition Function NOT defined for given letter_state_pair");
}



Automata::r_AFA::r_AFA(states_T number_of_states, std::vector<states_T> finishing_states, r_AFA_Transition_Function* transition_function) : number_of_states(number_of_states), finishing_states(finishing_states), transition_function(transition_function)
{
    for (states_T final_state : finishing_states)
    {
        assert(final_state < number_of_states && "All final states should be within the state range specified");
    }
}

bool Automata::r_AFA::accept(std::vector<alphabet_T> input)
{
    std::deque<alphabet_T> proper_input{};
    for (alphabet_T item : input)
        proper_input.push_back(item);
    return accept(proper_input);
}

bool Automata::r_AFA::accept(std::deque<alphabet_T>& word)
{
    boost::dynamic_bitset<> charcteristic_vector{ static_cast<size_t>(number_of_states) };
    for (states_T final_state : finishing_states)
        charcteristic_vector[number_of_states - 1 - final_state].flip();

    while (!word.empty())
    {
        alphabet_T letter{ word.back() };
        word.pop_back();
        boost::dynamic_bitset<> new_charcteristic_vector{ static_cast<size_t>(number_of_states) };

        for (states_T state{ 0 }; state < number_of_states; state++)
        {
            Pair at{ letter,state };
            new_charcteristic_vector[number_of_states-1 - state] = (*transition_function)[at]->eval(&charcteristic_vector);
        }
        charcteristic_vector = new_charcteristic_vector;
    }

    return (charcteristic_vector[number_of_states - 1 - starting_state] == true);
}

