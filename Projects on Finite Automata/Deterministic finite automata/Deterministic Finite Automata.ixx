module;
#include<unordered_map>
#include<unordered_set>

export module DFA;
import <vector>;
import <queue>;

export
{
	namespace automata
	{
		template <typename states_T, typename alphabet_T>
		struct state_letter_pair
		{
			states_T state;
			alphabet_T letter;

			size_t hash() const
			{
				if (static_cast<size_t>(state) > static_cast<size_t>(letter))
				{
					return static_cast<size_t>(state) * static_cast<size_t>(state) + static_cast<size_t>(state) + static_cast<size_t>(letter);
				}
				else
				{
					return static_cast<size_t>(letter) * static_cast<size_t>(letter) + static_cast<size_t>(state);
				}

			}

			bool operator==(const state_letter_pair< states_T, alphabet_T>& other) const {
				return state == other.state && letter == other.letter;
			}
		};

		template <typename states_T, typename alphabet_T>
		struct Rules
		{
			std::unordered_map<state_letter_pair<states_T, alphabet_T>, states_T> map{};

			void add(state_letter_pair<states_T, alphabet_T> state_and_letter, states_T next_state)
			{
				map[state_and_letter] = next_state;
			}

			states_T operator[](state_letter_pair<states_T, alphabet_T>& key)
			{
				if (auto search = map.find(key); search != map.end())
				{

					return (*search).second;
				}
				throw "No Mapping";
			}

		};

		template <typename states_T, typename alphabet_T>
		class DFA
		{
		public:

			std::queue<alphabet_T> current_word{};
			states_T state;

			DFA(states_T start_state, std::unordered_set<states_T> finish_states, Rules<states_T, alphabet_T> rule_set) : starting_state{ start_state }, finishing_states{ finish_states }, rules{ rule_set }
			{
			}

			//returns true if the DFA with this input reaches a finishing state. 
			//if it encounters a mapping that has not been defined or does not reach a finishing state it should return false 

			bool accept(std::vector<alphabet_T> input)
			{
				std::queue<alphabet_T> proper_input{};
				for (alphabet_T item : input)
					proper_input.push(item);
				return accept(proper_input);
			}

			bool accept(std::queue<alphabet_T>& input)
			{
				set_up(input);
				while (current_word.size() > 0) step();


				if (end()) return true;
				return false;
			}

			//reset state and set with an input word.
			void set_up(std::queue<alphabet_T>& input)
			{
				state = starting_state;
				current_word = input;
			}

			//get the DFA to make a single step.
			void step()
			{
				alphabet_T front{ current_word.front() };
				state_letter_pair<states_T,alphabet_T> pair{ state, front };
				state = rules[pair];
				current_word.pop();
			}

			// return true if a finishing state has been reached
			bool end()
			{
				if (finishing_states.contains(state))
					return true;
				return false;
			}
		private:
			states_T starting_state;
			std::unordered_set<states_T> finishing_states;

			Rules<states_T, alphabet_T> rules;

		};

	}


	template<typename states_T, typename alphabet_T>
	struct std::hash<automata::state_letter_pair< states_T, alphabet_T>>
	{
		std::size_t operator()(const automata::state_letter_pair< states_T, alphabet_T>& s) const noexcept
		{
			std::size_t h = s.hash();
			return h;
		}
	};
}

