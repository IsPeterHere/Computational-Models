#include <iostream>
#include <vector>
#include <Turing.h>


enum states
{
    start,
    end,
};

enum symbols
{
    blank,
    next,
    done,

};

int main()
{
    Turing::Rules<states,symbols> rules{};
    rules.add({ start,blank }, { end, blank , Turing::stay }); // add ending rule
    rules.add({ start, next }, { start, done, Turing::right }); // add a rule that results in movment 

    Turing::Turing_Machine<states, symbols, 100> machine(end, rules); //Create a Machine with 100 tape segments that will end when state "end" is reached and that uses our rule set

    std::vector<symbols> input{ next,next,next,next,next,next,blank,next };// Create a input, when given to the machine it will be inserted into the tape starting from the left most cell
    for (auto item: machine.boot_up(input)) // On the Machine halting the contents of the tape left of the head is outputted
        std::cout << item;

}
