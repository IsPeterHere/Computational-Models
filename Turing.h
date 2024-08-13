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

    template <typename sate_T, typename alphabet_T>
    struct State_Symbol
    {
        sate_T state;
        alphabet_T symbol;

        std::string toString() const {
            return std::to_string(state) + std::to_string(symbol);
        }

        size_t hash() const {
            return std::hash<std::string>()(toString());
        }

        bool operator==(const State_Symbol< sate_T, alphabet_T>& other) const {
            return state == other.state && symbol == other.symbol;
        }
    };

    template <typename sate_T, typename alphabet_T>
    struct State_Symbol_Movement
    {
        sate_T state;
        alphabet_T symbol;
        Movement move;
    };

    template <typename sate_T, typename alphabet_T>
    struct Rules
    {
        using SS = State_Symbol<sate_T, alphabet_T>;
        using SSM = State_Symbol_Movement<sate_T, alphabet_T>;

        std::unordered_map<SS, SSM> map{};

        void add(SS State_symbolKey, SSM State_symbol_MoveValue)
        {
            map[State_symbolKey] = State_symbol_MoveValue;
        }

        void add(sate_T StateKey, alphabet_T symbolKey, sate_T StateValue, alphabet_T symbolValue, Movement MovementValue)
        {
            map[{StateKey, symbolKey}] = { StateValue,symbolValue,MovementValue };
        }

        SSM& operator[](SS& key) { return map[key]; }
    };

    template <typename sate_T, typename alphabet_T, int tape_size_T>
    class Turing_Machine
    {
    public:

        using RuleSet = Rules<sate_T, alphabet_T>;
        using SS = State_Symbol<sate_T, alphabet_T>;
        using SSM = State_Symbol_Movement<sate_T, alphabet_T>;

        Turing_Machine(std::vector<alphabet_T>& input, sate_T finish_state, RuleSet& rules) 
            : input_sequence{ input },
              final_state{ finish_state },
              transformation_rules{ rules }
        {
            assert((std::size(input) < tape_size) && "Input sequence is longer than given tape length");
        }

        //The return will be a string containg charcters taken from the leftmost cell to the position if the machines head.
        std::vector<alphabet_T> boot_up()
        {
            reset();

            //The input is inserted into the tape from the leftmost cell.
            for (int i{}; i < std::ssize(input_sequence); i++)
            {
                tape[i] = input_sequence[i];
            }
            while (state != final_state)
            {
                step();
            }

            return std::vector<alphabet_T>(tape.begin(), tape.end() - (tape_size - position));
        }

    private:
        //Pre-Run-Time Setup
        std::vector<alphabet_T> input_sequence;
        const std::size_t tape_size{ static_cast<std::size_t>(tape_size_T) };
        sate_T final_state;
        RuleSet transformation_rules;

        //Run-Time Enviroment
        int position{ 0 };
        std::vector<alphabet_T> tape{ std::vector<alphabet_T>(tape_size) };
        sate_T state{};

        void reset()
        {
            position = 0;
            std::fill(tape.begin(), tape.end(), static_cast<alphabet_T>(0));
            state = static_cast<sate_T>(0);
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


template<typename sate_T, typename alphabet_T>
struct std::hash<Turing::State_Symbol< sate_T,  alphabet_T>>
{
    std::size_t operator()(const Turing::State_Symbol< sate_T,  alphabet_T>& s) const noexcept
    {
        std::size_t h = s.hash();
        return h;
    }
};