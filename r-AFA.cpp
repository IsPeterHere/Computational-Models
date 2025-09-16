#include"ComM.h"


bool Automata::Disjunctive_Normal_Term::eval(boost::dynamic_bitset<> inputs) const
{
    if (((inputs &= alpha) ^= beta).none())
        return 1;
    return 0;
}

void Automata::Disjunctive_Normal_Form::initDNF(std::shared_ptr < Boolean_Function > function, Automata::Disjunctive_Normal_Term* term)
{
    switch (function->type)
    {
    case Boolean_Function::Type::VAR:
        if (term == NULL)
        {
            Automata::Disjunctive_Normal_Term new_term{size};
            new_term.alpha[size - 1 - function->var1].flip();
            add_term(new_term);
        }
        else
        {
            term->alpha[size - 1 - function->var1].flip();
        }
        break;
    case Boolean_Function::Type::VAR_VAR:
        switch (function->operation)
        {
        case Boolean_Function::Operation::AND:
            if (term == NULL)
            {
                Automata::Disjunctive_Normal_Term new_term{ size };
                new_term.alpha[size - 1 - function->var1].flip();
                new_term.beta[size - 1 - function->var1].flip();
                new_term.alpha[size - 1 - function->var2].flip();
                new_term.beta[size - 1 - function->var2].flip();
                add_term(new_term);
            }
            else
            {
                term->alpha[size - 1 - function->var1].flip();
                term->beta[size - 1 - function->var1].flip();
                term->alpha[size - 1 - function->var2].flip();
                term->beta[size - 1 - function->var2].flip();
            }
            break;
        case Boolean_Function::Operation::OR:
            Automata::Disjunctive_Normal_Term new_term1{ size };
            new_term1.alpha[size - 1 - function->var1].flip();
            new_term1.beta[size - 1 - function->var1].flip();
            add_term(new_term1);

            Automata::Disjunctive_Normal_Term new_term2{ size };
            new_term2.alpha[size - 1 - function->var2].flip();
            new_term2.beta[size - 1 - function->var2].flip();
            add_term(new_term2);

        }
        break;
    case Boolean_Function::Type::VAR_FUNC:
        switch (function->operation)
        {
        case Boolean_Function::Operation::AND:
            if (term == NULL)
            {
                Automata::Disjunctive_Normal_Term* new_term{ new Automata::Disjunctive_Normal_Term(size) };
                new_term->alpha[size - 1 - function->var1].flip();
                new_term->beta[size - 1 - function->var1].flip();
                initDNF(function->func1, new_term);
                add_term(*new_term);
                delete new_term;
            }
            else
            {
                term->alpha[size - 1 - function->var1].flip();
                term->beta[size - 1 - function->var1].flip();
                initDNF(function->func1,term);
            }
            break;
        case Boolean_Function::Operation::OR:
            initDNF(function->func1);

            Automata::Disjunctive_Normal_Term new_term{ size };
            new_term.alpha[size - 1 - function->var1].flip();
            new_term.beta[size - 1 - function->var1].flip();
            add_term(new_term);
        }
        break;
    case Boolean_Function::Type::FUNC_FUNC:
        switch (function->operation)
        {
        case Boolean_Function::Operation::AND:
            if (term == NULL)
            {
                Automata::Disjunctive_Normal_Term* new_term{ new Automata::Disjunctive_Normal_Term(size) };
                initDNF(function->func1, new_term);
                initDNF(function->func2, new_term);
                add_term(*new_term);
                delete new_term;
            }
            else
            {
                initDNF(function->func1, term);
                initDNF(function->func2, term);
            }
            break;
        case Boolean_Function::Operation::OR:
            initDNF(function->func1);
            initDNF(function->func2);
        }
        break;
    }
}

Automata::Disjunctive_Normal_Form::Disjunctive_Normal_Form(size_t size, std::shared_ptr < Boolean_Function > function) : size(size)
{
    if (!function->isDNF())
        throw std::runtime_error("Given Boolean formula must be in DNF to initialize Disjunctive_Normal_Form object");

    initDNF(function);
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
    terms.push_back({ size });
}

void Automata::Disjunctive_Normal_Form::set_false()
{
    terms.clear();
    Disjunctive_Normal_Term DNT{ size };
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


void Automata::r_AFA_DNF_Transition_Function::add(states_T state, alphabet_T letter, Disjunctive_Normal_Form* disjuctive_form)
{
    letter_state_pair pair{ letter,state };
    map[pair] = disjuctive_form;
}

Automata::Disjunctive_Normal_Form* Automata::r_AFA_DNF_Transition_Function::operator[](letter_state_pair& key)
{
    auto search{ map.find(key) };
    if (search != map.end())
        return (*search).second;
    throw std::runtime_error("Transition Function NOT defined for given letter_state_pair");
}


void Automata::r_AFA_Transition_Function::add(states_T state, alphabet_T letter, std::shared_ptr < Boolean_Function > disjuctive_form)
{
    letter_state_pair pair{ letter,state };
    map[pair] = disjuctive_form;
}

std::shared_ptr < Boolean_Function > Automata::r_AFA_Transition_Function::operator[](letter_state_pair& key)
{
    auto search{ map.find(key) };
    if (search != map.end())
        return (*search).second;
    throw std::runtime_error("Transition Function NOT defined for given letter_state_pair");
}


Automata::r_AFA::r_AFA(states_T number_of_states, std::vector<states_T> finishing_states, r_AFA_DNF_Transition_Function* transition_function) : number_of_states(number_of_states), finishing_states(finishing_states), transition_function(transition_function), type(Automata::r_AFA::Type::DNF)
{
    for (states_T final_state : finishing_states)
    {
        assert(final_state < number_of_states && "All final states should be within the state range specified by number_of_states");
    }
}

Automata::r_AFA::r_AFA(states_T number_of_states, std::vector<states_T> finishing_states, r_AFA_Transition_Function* transition_function) : number_of_states(number_of_states), finishing_states(finishing_states), transition_function(transition_function), type(Automata::r_AFA::Type::BOOLEAN_FORMULA)
{
    for (states_T final_state : finishing_states)
    {
        assert(final_state < number_of_states && "All final states should be within the state range specified by number_of_states");
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
    switch (type)
    {
    case Type::DNF:
        return accept_DNF(word);
    case Type::BOOLEAN_FORMULA:
        return accept_BOOLEAN_FORMULA(word);
    }
}

bool Automata::r_AFA::accept_DNF(std::deque<alphabet_T>& word)
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
            new_charcteristic_vector[number_of_states - 1 - state] = (*static_cast<r_AFA_DNF_Transition_Function*>(transition_function))[at]->eval(&charcteristic_vector);
        }
        charcteristic_vector = new_charcteristic_vector;
    }

    return (charcteristic_vector[number_of_states - 1 - starting_state] == true);
}
bool Automata::r_AFA::accept_BOOLEAN_FORMULA(std::deque<alphabet_T>& word)
{
    size_t size{ static_cast<size_t>(number_of_states) };
    boost::dynamic_bitset<> charcteristic_vector{ size };
    for (states_T final_state : finishing_states)
        charcteristic_vector[number_of_states - 1 - final_state].flip();

    while (!word.empty())
    {
        alphabet_T letter{ word.back() };
        word.pop_back();
        boost::dynamic_bitset<> new_charcteristic_vector{ size };
        auto eval_func = [charcteristic_vector,size](Boolean_Function::variable_repr_T var) {  return static_cast<bool>(charcteristic_vector[size-var-1]); };
        for (states_T state{ 0 }; state < number_of_states; state++)
        {
            Pair at{ letter,state };
            new_charcteristic_vector[number_of_states - 1 - state] = (*static_cast<r_AFA_Transition_Function*>(transition_function))[at]->eval(eval_func);
        }
        charcteristic_vector = new_charcteristic_vector;
    }

    return (charcteristic_vector[number_of_states - 1 - starting_state] == true);
}