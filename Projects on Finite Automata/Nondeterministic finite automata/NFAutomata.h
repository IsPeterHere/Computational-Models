#ifndef NFAutomata
#define NFAutomata

#include<unordered_map>
#include<Eigen/Sparse>
#include<vector>
#include<queue>

namespace automata
{
	template <typename states_T>
	struct State_state_pair
	{
		states_T state1;
		states_T state2;

		size_t hash() const
		{
			if (static_cast<size_t>(state1) > static_cast<size_t>(state2))
			{
				return static_cast<size_t>(state1) * static_cast<size_t>(state1) + static_cast<size_t>(state1) + static_cast<size_t>(state2);
			}
			else
			{
				return static_cast<size_t>(state2) * static_cast<size_t>(state2) + static_cast<size_t>(state1);
			}

		}

		bool operator==(const State_state_pair< states_T>& other) const {
			return state1 == other.state1 && state2 == other.state2;
		}
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
			while (word.size() > 0) identifying_morphism = identifying_morphism * step( word);
				
			Eigen::SparseMatrix<int> result = (initial_vec * identifying_morphism * final_vec);
			return (result.coeffRef(0, 0) > 0);

		}

	private:
		std::queue<alphabet_T> input_word;
		NFARules<states_T, alphabet_T> rules;

		Eigen::SparseMatrix<int> initial_vec{ 1, STATES_LENGHT };
		Eigen::SparseMatrix<int> final_vec{ STATES_LENGHT, 1 };

		Eigen::SparseMatrix<int> step(std::queue<alphabet_T>&  word)
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
struct std::hash<automata::State_state_pair< states_T>>
{
	std::size_t operator()(const automata::State_state_pair< states_T>& s) const noexcept
	{
		std::size_t h = s.hash();
		return h;
	}
};

#endif
