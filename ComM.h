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
        ACCEPT,
        REJECT,

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

        using RuleSet = Rules;
        using SS = State_Symbol;
        using SSM = State_Symbol_Movement;

        Turing_Machine(RuleSet& rules, size_t start_state);

        //The return will be a vector containing symbols taken from the leftmost cell to the position if the machines head.
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
        boost::dynamic_bitset<> alpha; //if x or ¬x appears
        boost::dynamic_bitset<> beta;  //if x appears

        bool eval(boost::dynamic_bitset<>* inputs) const;
    };

    class Disjunctive_Normal_Form
    {
    public:
        Disjunctive_Normal_Form(size_t size) : size(size) {}

        void add_term(Disjunctive_Normal_Term term);
        void set_true();
        void set_false();
        bool eval(boost::dynamic_bitset<>* inputs);

    private:
        size_t size;
        std::vector<Disjunctive_Normal_Term> terms;

    };


    class Boolean_Function
    {
    public:

        enum class Type
        {
            STATE,
            FUNC,

            STATE_STATE,
            STATE_FUNC,
            FUNC_FUNC
        };

        enum class Operation
        {
            AND,
            OR,
            NOT
        };

        using states_T = int;

        Boolean_Function(states_T term, Operation operation = Operation::NOT);
        Boolean_Function(Boolean_Function* term, Operation operation);
        Boolean_Function(states_T term1, states_T term2, Operation operation) : type(Type::STATE_STATE), operation(operation), state1(term1), state2(term2){}
        Boolean_Function(states_T term1, Boolean_Function* term2, Operation operation) : type(Type::STATE_FUNC), operation(operation), state1(term1), func2(term2){}
        Boolean_Function(Boolean_Function* term2, states_T term1, Operation operation) : type(Type::STATE_FUNC), operation(operation), state1(term1), func2(term2) {}
        Boolean_Function(Boolean_Function* term1, Boolean_Function* term2, Operation operation) : type(Type::FUNC_FUNC), operation(operation), func1(term1), func2(term2){}

        Disjunctive_Normal_Form convert_to_disjunctive_normal_form();

    private:
        Type type;
        Operation operation;

        [[maybe_unused]] states_T state1{};
        [[maybe_unused]] states_T state2{};

        [[maybe_unused]] Boolean_Function* func1{ NULL };
        [[maybe_unused]] Boolean_Function* func2{ NULL };
    };




    class r_AFA_Transition_Function
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

    class r_AFA
    {
    public:

        using alphabet_T = int;
        using states_T = int;

        r_AFA(states_T number_of_states, std::vector<states_T> finishing_states, r_AFA_Transition_Function* transition_function);
        bool accept(std::vector<alphabet_T> input);
        bool accept(std::deque<alphabet_T>& word);

    private:
        states_T number_of_states;
        const states_T starting_state{ 0 };
        std::vector<states_T> finishing_states;
        r_AFA_Transition_Function* transition_function;
    };

}
