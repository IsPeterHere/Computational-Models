#include"ComM.h"

size_t Automata::State_state_pair::hash() const
{
	if (static_cast<size_t>(state1) > static_cast<size_t>(state2))
		return static_cast<size_t>(state1) * static_cast<size_t>(state1) + static_cast<size_t>(state1) + static_cast<size_t>(state2);
	else
		return static_cast<size_t>(state2) * static_cast<size_t>(state2) + static_cast<size_t>(state1);

}

std::vector<Automata::State_state_pair> Automata::NFARules::operator[](alphabet_T& key)
{
	auto search{ map.find(key) };
	if (search != map.end())
		return (*search).second;
	return {};
}


Automata::NFA::NFA(int number_of_states, std::vector<states_T> starting_states, std::vector<states_T> finishing_states, NFARules rule_set) : rules{ rule_set },number_of_states(number_of_states), initial_vec({ 1, number_of_states }), final_vec({ number_of_states, 1 })
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



bool Automata::NFA::accept(std::vector<alphabet_T> input)
{
	std::queue<alphabet_T> proper_input{};
	for (alphabet_T item : input)
		proper_input.push(item);
	return accept(proper_input);
}

bool Automata::NFA::accept(std::queue<alphabet_T>& word)
{

	Eigen::SparseMatrix<int> identifying_morphism{ step(word) };
	while (word.size() > 0) identifying_morphism = identifying_morphism * step(word);

	Eigen::SparseMatrix<int> result = (initial_vec * identifying_morphism * final_vec);
	return (result.coeffRef(0, 0) > 0);

}

Eigen::SparseMatrix<int> Automata::NFA::step(std::queue<alphabet_T>& word)
{
	alphabet_T letter{ word.front() };
	word.pop();

	std::vector<Eigen::Triplet<double>> tripletList;
	tripletList.reserve(rules[letter].size());
	for (State_state_pair state_pair : rules[letter])
		tripletList.push_back({ static_cast<int>(state_pair.state1), static_cast<int>(state_pair.state2), 1 });

	Eigen::SparseMatrix<int> character_morphism{ number_of_states, number_of_states };
	character_morphism.setFromTriplets(tripletList.begin(), tripletList.end());
	return character_morphism;
}

template<>
struct std::hash<Automata::State_state_pair>
{
	std::size_t operator()(const Automata::State_state_pair& s) const noexcept
	{
		std::size_t h = s.hash();
		return h;
	}
};

