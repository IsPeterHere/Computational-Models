#pragma once
#include<unordered_map>
#include<unordered_set>
#include<Eigen/Sparse>
#include<vector>
#include<queue>
#include<stdexcept>
#include<cassert>





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
	struct State_state_pair
	{
		size_t state1;
		size_t state2;

		size_t hash() const;

		bool operator==(const State_state_pair& other) const { return state1 == other.state1 && state2 == other.state2; }
	};


	struct NFARules
	{
		using State_Transitions = std::vector<State_state_pair>;
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

        NFA(int number_of_states, std::vector<states_T> starting_states, std::vector<states_T> finishing_states, NFARules rule_set);

        bool accept(std::vector<alphabet_T> input);
        bool accept(std::queue<alphabet_T>& word);

	private:
        int number_of_states;
		std::queue<alphabet_T> input_word;
		NFARules rules;

		Eigen::SparseMatrix<int> initial_vec;
		Eigen::SparseMatrix<int> final_vec;

        Eigen::SparseMatrix<int> step(std::queue<alphabet_T>& word);
	};

}
