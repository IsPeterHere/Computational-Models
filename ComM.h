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
        State_Symbol_Movement& operator[](State_Symbol& key)
        {
            if (auto search = map.find(key); search != map.end()) return (*search).second;
            throw std::runtime_error("Missing Rule");
        }
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
	template <typename states_T>
	struct State_state_pair
	{
		states_T state1;
		states_T state2;

		size_t hash() const;

		bool operator==(const State_state_pair< states_T>& other) const { return state1 == other.state1 && state2 == other.state2; }
	};


	template <typename states_T, typename alphabet_T>
	struct NFARules
	{
		using State_Transitions = std::vector<State_state_pair<states_T>>;

		std::unordered_map<alphabet_T, std::vector<State_state_pair<states_T>>> map{};

		void add(alphabet_T letter, std::vector<State_state_pair<states_T>>& state_pairs)
		{
			map[letter] = state_pairs;
		}

		std::vector<State_state_pair<states_T>> operator[](alphabet_T& key)
		{
			auto search{ map.find(key) };
			if (search != map.end())
			{
				return (*search).second;
			}
			return {};
		}

	};

	template <typename states_T, typename alphabet_T, int STATES_LENGHT>
	class NFA
	{
	public:

		NFA(std::vector<states_T> starting_states, std::vector<states_T> finishing_states, NFARules<states_T, alphabet_T> rule_set) : rules{ rule_set }
		{
			std::vector<Eigen::Triplet<double>> starting_triplet_List;
			starting_triplet_List.reserve(starting_states.size());
			for (states_T state : starting_states)
				starting_triplet_List.push_back({ 0, static_cast<int>(state), 1 });

			initial_vec.setFromTriplets(starting_triplet_List.begin(), starting_triplet_List.end());

			std::vector<Eigen::Triplet<double>> finishing_triplet_List;
			finishing_triplet_List.reserve(finishing_states.size());
			for (states_T state : finishing_states)
				finishing_triplet_List.push_back({ static_cast<int>(state),0, 1 });

			final_vec.setFromTriplets(finishing_triplet_List.begin(), finishing_triplet_List.end());
		}

		bool accept(std::vector<alphabet_T> input)
		{
			std::queue<alphabet_T> proper_input{};
			for (alphabet_T item : input)
				proper_input.push(item);
			return accept(proper_input);
		}

		bool accept(std::queue<alphabet_T>& word)
		{

			Eigen::SparseMatrix<int> identifying_morphism{ step(word) };
			while (word.size() > 0) identifying_morphism = identifying_morphism * step(word);

			Eigen::SparseMatrix<int> result = (initial_vec * identifying_morphism * final_vec);
			return (result.coeffRef(0, 0) > 0);

		}

	private:
		std::queue<alphabet_T> input_word;
		NFARules<states_T, alphabet_T> rules;

		Eigen::SparseMatrix<int> initial_vec{ 1, STATES_LENGHT };
		Eigen::SparseMatrix<int> final_vec{ STATES_LENGHT, 1 };

		Eigen::SparseMatrix<int> step(std::queue<alphabet_T>& word)
		{
			alphabet_T letter{ word.front() };
			word.pop();

			std::vector<Eigen::Triplet<double>> tripletList;
			tripletList.reserve(rules[letter].size());
			for (State_state_pair<states_T> state_pair : rules[letter])
				tripletList.push_back({ static_cast<int>(state_pair.state1), static_cast<int>(state_pair.state2), 1 });

			Eigen::SparseMatrix<int> character_morphism{ STATES_LENGHT, STATES_LENGHT };
			character_morphism.setFromTriplets(tripletList.begin(), tripletList.end());
			return character_morphism;
		}
	};

}


template<typename states_T>
struct std::hash<Automata::State_state_pair< states_T>>
{
	std::size_t operator()(const Automata::State_state_pair< states_T>& s) const noexcept
	{
		std::size_t h = s.hash();
		return h;
	}
};