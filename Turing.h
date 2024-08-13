#include<vector>
#include<cassert>
#include<unordered_map>
#include< string >

namespace Turing
{
    enum Movement
    {
        stay,
        left,
        right,
    };

    template <typename sate_set_T, typename symbol_set_T>
    struct State_Symbol
    {
        sate_set_T state;
        symbol_set_T symbol;

        std::string toString() const {
            return std::to_string(state) + std::to_string(symbol);
        }

        size_t hash() const {
            return std::hash<std::string>()(toString());
        }

        bool operator==(const State_Symbol< sate_set_T, symbol_set_T>& other) const {
            return state == other.state && symbol == other.symbol;
        }
    };

    template <typename sate_set_T, typename symbol_set_T>
    struct State_Symbol_Movement
    {
        sate_set_T state;
        symbol_set_T symbol;
        Movement move;
    };

    template <typename sate_set_T, typename symbol_set_T>
    struct Rules
    {
        using SS = State_Symbol<sate_set_T, symbol_set_T>;
        using SSM = State_Symbol_Movement<sate_set_T, symbol_set_T>;

        std::unordered_map<SS, SSM> map{};

        void add(SS State_symbolKey, SSM State_symbol_MoveValue)
        {
            map[State_symbolKey] = State_symbol_MoveValue;
        }

        void add(sate_set_T StateKey, symbol_set_T symbolKey, sate_set_T StateValue, symbol_set_T symbolValue, Movement MovementValue)
        {
            map[{StateKey, symbolKey}] = { StateValue,symbolValue,MovementValue };
        }

        SSM& operator[](SS& key) { return map[key]; }
    };

    template <typename sate_set_T, typename symbol_set_T, int tape_size_T>
    class Turing_Machine
    {
    public:

        using RuleSet = Rules<sate_set_T, symbol_set_T>;
        using SS = State_Symbol<sate_set_T, symbol_set_T>;
        using SSM = State_Symbol_Movement<sate_set_T, symbol_set_T>;

        Turing_Machine(sate_set_T finish_state, RuleSet& rules) 
            :
              final_state{ finish_state },
              transformation_rules{ rules }
        {
        }

        //The return will be a string containg charcters taken from the leftmost cell to the position if the machines head.
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
        std::vector<symbol_set_T>* input_sequence {};
        const std::size_t tape_size{ static_cast<std::size_t>(tape_size_T) };
        sate_set_T final_state;
        RuleSet transformation_rules;

        //Run-Time Enviroment
        int position{ 0 };
        std::vector<symbol_set_T> tape{ std::vector<symbol_set_T>(tape_size) };
        sate_set_T state{};

        void reset()
        {
            position = 0;
            std::fill(tape.begin(), tape.end(), static_cast<symbol_set_T>(0));
            state = static_cast<sate_set_T>(0);
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
                assert((position > 0) && "Head attempted to move out of tape : left");
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
struct std::hash<Turing::State_Symbol< sate_set_T,  symbol_set_T>>
{
    std::size_t operator()(const Turing::State_Symbol< sate_set_T,  symbol_set_T>& s) const noexcept
    {
        std::size_t h = s.hash();
        return h;
    }
};