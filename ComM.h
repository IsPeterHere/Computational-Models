#pragma once
#include<unordered_map>
#include<unordered_set>
#include<Eigen/Sparse>
#include<vector>
#include<array>
#include<queue>
#include<deque>
#include<stdexcept>
#include<cassert>
#include <boost/dynamic_bitset.hpp>
#include <iostream>
#include <memory>

#include"BooleanFunctions.h"

using namespace std::string_literals;

namespace Turing
{
    //In this model the Turing machine can move 1 left/right or stay in place.
    enum Movement :size_t
    {
        STAY,
        LEFT,
        RIGHT,
    };

    //The First Symbols Will always Correspond to the following
    enum Fundemental_Symbols :size_t
    {
        BLANK,
        LEFTMOST,

        NUMBER_OF_FUNDEMENTAL_SYMBOLS
    };

    //The First States Will always Correspond to the following
    enum Fundemental_States :size_t
    {
        HALT,

        NUMBER_OF_FUNDEMENTAL_STATES
    };

    //The Turing machine works by mapping (state,symbol) pairs to a new (state,symbol,movement) triple.
    struct State_Symbol
    {
        size_t state;
        size_t symbol;

        //The state symbol pair must be hash-able so it can be used as a key in the rules map
        size_t hash() const;

        bool operator==(const State_Symbol& other) const { return state == other.state && symbol == other.symbol; }
    };
}

template<>
struct std::hash<Turing::State_Symbol>
{
    std::size_t operator()(const Turing::State_Symbol& s) const noexcept
    {
        std::size_t h = s.hash();
        return h;
    }
};

namespace Turing
{
    struct State_Symbol_Movement
    {
        size_t state;
        size_t symbol;
        size_t move;
    };

    //The rules class contains and managers the unordered map between the pairs and the triples (see above)
    struct Rules
    {
        std::unordered_map<State_Symbol, State_Symbol_Movement> map{};

        //add new entry
        void add(State_Symbol State_symbolKey, State_Symbol_Movement State_symbol_MoveValue);

        //add new entry with a list of state keys (i.e one new rule for every state)
        void add(std::vector<size_t>& State_Keys, size_t Symbol_key, State_Symbol_Movement State_symbol_MoveValue);

        //add new entry with a list of state keys and state value (i.e one new rule for every pair of state)
        void add(std::vector<size_t>& State_Keys, size_t Symbol_key, std::vector<size_t>& State_Values, size_t Symbol_Value, size_t Move_Value);

        //operator [] to access rules with a pair and return a triple
        State_Symbol_Movement& operator[](State_Symbol& key);
    };

    class Turing_Machine
    {
    public:
        
        //A Traditional Turing Machine, 
        //Specifically derived from definition in Papadimitriou, Christos H .. Computational Complexity. United Kingdom, Addison-Wesley, 1993.

        using RuleSet = Rules;
        using SS = State_Symbol;
        using SSM = State_Symbol_Movement;

        Turing_Machine(RuleSet& rules, size_t start_state);

        //The return will be a vector containing symbols taken from the leftmost cell to the position of the machines head.
        void boot_up(std::vector<size_t>& input);
        void run();

        std::vector<size_t>* get_tape(){ return &tape; }

    private:
        //Pre-Run-Time Setup
        std::vector<size_t>* input_sequence{};
        size_t start_state;
        RuleSet transformation_rules;

        //Run-Time Environment
        int position{ 0 };
        std::vector<size_t> tape;
        size_t state{};

        void reset();
        void step();
        bool check_done();
    };



    class Bounded_Turing_Machine
    {

    };
}


namespace Automata
{
    struct Pair
    {
        using states_T = size_t;

        states_T item1;
        states_T item2;

        size_t hash() const;

        bool operator==(const Pair& other) const { return item1 == other.item1 && item2 == other.item2; }
    };

}
template<>
struct std::hash<Automata::Pair>
{
    std::size_t operator()(const Automata::Pair& s) const noexcept
    {
        std::size_t h = s.hash();
        return h;
    }
};

namespace Automata
{
	struct NFARules
	{
		using State_Transitions = std::vector<Pair>;
        using alphabet_T = size_t;

		std::unordered_map<alphabet_T, State_Transitions> map{};

        void add(alphabet_T letter, State_Transitions& state_pairs) { map[letter] = state_pairs; }

        State_Transitions operator[](alphabet_T& key);

	};

	class NFA
	{
	public:
        using alphabet_T = size_t;
        using states_T = size_t;

        NFA(int number_of_states, std::vector<states_T> starting_states, std::vector<states_T> finishing_states, NFARules* rule_set);

        bool accept(std::vector<alphabet_T> input);
        bool accept(std::queue<alphabet_T>& word);

	private:
        int number_of_states;
		std::queue<alphabet_T> input_word;
		NFARules* rules;

		Eigen::SparseMatrix<int> initial_vec;
		Eigen::SparseMatrix<int> final_vec;

        Eigen::SparseMatrix<int> step(std::queue<alphabet_T>& word);
	};





    struct Disjunctive_Normal_Term
    {
        Disjunctive_Normal_Term(size_t size)
        {
            alpha = boost::dynamic_bitset<>(size);
            beta = boost::dynamic_bitset<>(size);
        }

        boost::dynamic_bitset<> alpha; // 1 if x or ¬x appears else 0
        boost::dynamic_bitset<> beta;  // 1 if x appears else 0

        bool eval(boost::dynamic_bitset<> inputs) const;
    };

    class Disjunctive_Normal_Form
    {
    public:
        Disjunctive_Normal_Form(size_t size, std::shared_ptr < Boolean_Function >);
        Disjunctive_Normal_Form(size_t size) : size(size) {}

        void add_term(Disjunctive_Normal_Term term);
        void set_true(); //i.e a tautology
        void set_false(); //i.e a contradiction
        bool eval(boost::dynamic_bitset<>* inputs);


    private:
        size_t size;
        std::vector<Disjunctive_Normal_Term> terms;
        void initDNF(std::shared_ptr < Boolean_Function > function, Automata::Disjunctive_Normal_Term* term = NULL);
    };

    class r_AFA_DNF_Transition_Function
    {
    public:
        using alphabet_T = int;
        using states_T = int;
        using letter_state_pair = Pair;

        void add(states_T state, alphabet_T letter, Disjunctive_Normal_Form* disjuctive_form);
        Disjunctive_Normal_Form* operator[](letter_state_pair& key);

    private:

        std::unordered_map<letter_state_pair, Disjunctive_Normal_Form*> map{};
    };

    class r_AFA_Transition_Function
    {
    public:
        using alphabet_T = int;
        using states_T = int;
        using letter_state_pair = Pair;

        void add(states_T state, alphabet_T letter, std::shared_ptr < Boolean_Function > formula);
        std::shared_ptr < Boolean_Function > operator[](letter_state_pair& key);

    private:

        std::unordered_map<letter_state_pair, std::shared_ptr < Boolean_Function >> map{};
    };

    class r_AFA
    {
    public:
        enum class Type
        {
            DNF,
            BOOLEAN_FORMULA
        };

        using alphabet_T = int;
        using states_T = int;

        r_AFA(states_T number_of_states, std::vector<states_T> finishing_states, r_AFA_DNF_Transition_Function* transition_function);
        r_AFA(states_T number_of_states, std::vector<states_T> finishing_states, r_AFA_Transition_Function* transition_function);

        bool accept(std::vector<alphabet_T> input);
        bool accept(std::deque<alphabet_T>& word);

    private:
        Type type;

        states_T number_of_states;
        const states_T starting_state{ 0 };
        std::vector<states_T> finishing_states;

        void* transition_function;

        bool accept_DNF(std::deque<alphabet_T>& word);
        bool accept_BOOLEAN_FORMULA(std::deque<alphabet_T>& word);
    };

}

namespace L_Systems
{
    using alphabet_T = int;

    class Rules
    {
    public:
        Rules(alphabet_T alphabet_size) : alphabet_size(alphabet_size) 
        {
            rules.resize(alphabet_size);
        }

        void add(alphabet_T from, std::vector<alphabet_T> to) 
        {
            rules[from] = to;
        }
        std::vector<alphabet_T>* operator[](alphabet_T key)
        {
            return &rules[key];
        }
    private:
        alphabet_T alphabet_size;
        std::vector<std::vector<alphabet_T>> rules;
    };

    class L_System
    {
    public:
        L_System(Rules* rule_set, std::vector<alphabet_T> starting_word) : rule_set(rule_set), starting_word(starting_word)
        {
            reset();
        }

        void run(int steps)
        {
            for (int i = 0; i < steps; i++)
            {
                step();
            }
        }

        void reset()
        {
            word = std::vector<alphabet_T>(starting_word);
        }

        std::vector<alphabet_T> word{};

    private:
        Rules* rule_set;
        std::vector<alphabet_T> starting_word;

        void step()
        {
           std::vector<alphabet_T> new_word{};

           for (alphabet_T letter : word)
           {
               std::vector<alphabet_T>* expansion{ (*rule_set)[letter] };
               for (alphabet_T new_letter : *expansion) new_word.push_back(new_letter);
           }

           word.swap(new_word);
        }
    };
}