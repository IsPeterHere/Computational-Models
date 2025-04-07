#pragma once

#include<vector>
#include<cassert>
#include<unordered_map>
#include<stdexcept>


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

        //The state symbol pair must be hashable so it can be used as a key in the rules map
        size_t hash() const {
            if (state > symbol)
            {
                return state * state + state + symbol;
            }
            else
            {
                return symbol * symbol + state;
            }

        }

        bool operator==(const State_Symbol& other) const {
            return state == other.state && symbol == other.symbol;
        }
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
        void add(State_Symbol State_symbolKey, State_Symbol_Movement State_symbol_MoveValue)
        {
            map[State_symbolKey] = State_symbol_MoveValue;
        }
        //add new entry with a list of state keys (i.e one new rule for every state)
        void add(std::vector<size_t>& State_Keys, size_t Symbol_key, State_Symbol_Movement State_symbol_MoveValue)
        {
            for (size_t State_Key : State_Keys)
                map[{State_Key, Symbol_key}] = State_symbol_MoveValue;
        }
        //add new entry with a list of state keys and state value (i.e one new rule for every pair of state)
        void add(std::vector<size_t>& State_Keys, size_t Symbol_key, std::vector<size_t>& State_Values, size_t Symbol_Value, size_t Move_Value)
        {
            for (int i{}; i < std::ssize(State_Keys); ++i)
                map[{State_Keys[i], Symbol_key}] = { State_Values[i],Symbol_Value,Move_Value };
        }

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

        Turing_Machine(RuleSet& rules, size_t start_state)
            :
            transformation_rules{ rules },
            start_state{ start_state }
        {
        }

        //The return will be a vector containg symbols taken from the leftmost cell to the position if the machines head.
        void boot_up(std::vector<size_t>& input)
        {
            input_sequence = &input;
            tape = std::vector<size_t>(std::ssize(*input_sequence));

            reset();

            //The input is inserted into the tape from the leftmost cell.
            for (int i{}; i < std::ssize(*input_sequence); i++)
            {
                tape[i] = (*input_sequence)[i];
            }
        }

        void run()
        {
            while (!check_done()) step();
        }

        std::vector<size_t>* get_tape()
        {
            return &tape;
        }

    private:
        //Pre-Run-Time Setup
        std::vector<size_t>* input_sequence{};
        size_t start_state;
        RuleSet transformation_rules;

        //Run-Time Enviroment
        int position{ 0 };
        std::vector<size_t> tape;
        size_t state{};

        void reset()
        {
            position = 0;
            std::fill(tape.begin(), tape.end(), static_cast<size_t>(0));
            state = start_state;
        }

        void step()
        {
            SS current_step{ state,tape[position] };
            SSM next_step{ transformation_rules[current_step] };

            state = next_step.state;
            tape[position] = next_step.symbol;

            switch (static_cast<Movement>(next_step.move))
            {
            case(Movement::STAY):
                break;
            case(Movement::LEFT):
                --position;
                assert((position >= 0) && "Head attempted to move out of tape : left");
                break;
            case(Movement::RIGHT):
                ++position;
                if (position >= std::ssize(tape)) tape.push_back(BLANK);
                break;
            };

        }

        bool check_done()
        {
            switch (state)
            {
            case HALT:
            case ACCEPT:
            case REJECT:
                return true;
            default:
                return false;
            }
        }


    };
}

