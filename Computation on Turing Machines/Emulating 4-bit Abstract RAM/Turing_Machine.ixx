module;
#include<unordered_map>

export module Turing_Machine;

import <vector>;
import <cassert>;

export
{


    namespace Turing
    {
        //In this model the Turing machine can move 1 left/right or stay in place.
        enum Movement
        {
            stay,
            left,
            right,
        };

        //The Turing machine works by mapping (state,symbol) pairs to a new (state,symbol,movement) triple.
        template <typename sate_set_T, typename symbol_set_T>
        struct State_Symbol
        {
            sate_set_T state;
            symbol_set_T symbol;

            //The state symbol pair must be hashable so it can be used as a key in the rules map
            size_t hash() const {
                if (static_cast<size_t>(state) > static_cast<size_t>(symbol))
                {
                    return static_cast<size_t>(state) * static_cast<size_t>(state) + static_cast<size_t>(state) + static_cast<size_t>(symbol);
                }
                else
                {
                    return static_cast<size_t>(symbol) * static_cast<size_t>(symbol) + static_cast<size_t>(state);
                }

            }

            bool operator==(const State_Symbol< sate_set_T, symbol_set_T>& other) const {
                return state == other.state && symbol == other.symbol;
            }
        };

        //~~The Turing machine works by mapping (state,symbol) pairs to a new (state,symbol,movement) triple.
        template <typename sate_set_T, typename symbol_set_T>
        struct State_Symbol_Movement
        {
            sate_set_T state;
            symbol_set_T symbol;
            Movement move;
        };

        //The rules class contains and managers the unordered map between the pairs and the triples (see above)
        template <typename sate_set_T, typename symbol_set_T>
        struct Rules
        {
            using SS = State_Symbol<sate_set_T, symbol_set_T>;
            using SSM = State_Symbol_Movement<sate_set_T, symbol_set_T>;

            std::unordered_map<SS, SSM> map{};

            //add new entry
            void add(SS State_symbolKey, SSM State_symbol_MoveValue)
            {
                map[State_symbolKey] = State_symbol_MoveValue;
            }
            //add new entry with a list of state keys (i.e one new rule for every state)
            void add(std::vector<sate_set_T>& State_Keys, symbol_set_T Symbol_key, SSM State_symbol_MoveValue)
            {
                for (SS State_Key : State_Keys)
                    map[{State_Key, Symbol_key}] = State_symbol_MoveValue;
            }
            //add new entry with a list of state keys and state value (i.e one new rule for every pair of state)
            void add(std::vector<sate_set_T>& State_Keys, symbol_set_T Symbol_key, std::vector<sate_set_T>& State_Values, symbol_set_T Symbol_Value, Movement Move_Value)
            {
                for (int i{}; i < std::ssize(State_Keys); ++i)
                    map[{State_Keys[i], Symbol_key}] = { State_Values[i],Symbol_Value,Move_Value };
            }

            //operator [] to access rules with a pair and return a triple
            SSM& operator[](SS& key)
            {
                if (auto search = map.find(key); search != map.end())
                {

                    return (*search).second;
                }
            }
        };

        template <typename sate_set_T, typename symbol_set_T, int tape_size_T>
        class Turing_Machine
        {
        public:

            using RuleSet = Rules<sate_set_T, symbol_set_T>;
            using SS = State_Symbol<sate_set_T, symbol_set_T>;
            using SSM = State_Symbol_Movement<sate_set_T, symbol_set_T>;

            Turing_Machine(sate_set_T starting_state, sate_set_T finishing_state, RuleSet& rules)
                :
                start_state{ starting_state },
                final_state{ finishing_state },
                transformation_rules{ rules }
            {
            }

            //The return will be a vector containg symbols taken from the leftmost cell to the position if the machines head.
            std::vector<symbol_set_T> boot_up(std::vector<symbol_set_T>& input)
            {
                assert((std::size(input) < tape_size) && "Input sequence is longer than given tape length");
                input_sequence = &input;

                reset();

                //The input is inserted into the tape from the leftmost cell.
                for (int i{}; i < std::ssize(*input_sequence); i++)
                {
                    tape[i] = (*input_sequence)[i];
                }
                while (state != final_state)
                {
                    step();
                }

                return std::vector<symbol_set_T>(tape.begin(), tape.end() - (tape_size - position));
            }

        private:
            //Pre-Run-Time Setup
            std::vector<symbol_set_T>* input_sequence{};
            const std::size_t tape_size{ static_cast<std::size_t>(tape_size_T) };
            sate_set_T final_state;
            sate_set_T start_state;
            RuleSet transformation_rules;

            //Run-Time Enviroment
            int position{ 0 };
            std::vector<symbol_set_T> tape{ std::vector<symbol_set_T>(tape_size) };
            sate_set_T state{};

            void reset()
            {
                position = 0;
                std::fill(tape.begin(), tape.end(), static_cast<symbol_set_T>(0));
                state = start_state;
            }

            void step()
            {
                SS current_step{ state,tape[position] };
                SSM next_step{ transformation_rules[current_step] };

                state = next_step.state;
                tape[position] = next_step.symbol;

                switch (next_step.move)
                {
                case(Movement::stay):
                    break;
                case(Movement::left):
                    --position;
                    assert((position >= 0) && "Head attempted to move out of tape : left");
                    break;
                case(Movement::right):
                    ++position;
                    assert((position < tape_size) && "Head attempted to move out of tape : right");
                    break;
                };

            }


        };
    }

    template<typename sate_set_T, typename symbol_set_T>
    struct std::hash<Turing::State_Symbol< sate_set_T, symbol_set_T>>
    {
        std::size_t operator()(const Turing::State_Symbol< sate_set_T, symbol_set_T>& s) const noexcept
        {
            std::size_t h = s.hash();
            return h;
        }
    };
}