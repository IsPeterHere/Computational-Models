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
    std::vector<symbols> input{next,next,next,next,next,next ,blank,next};

    Turing::Rules<states,symbols> rules{};
    rules.add(start,blank,end,blank,Turing::stay);
    rules.add(start, next, start, done, Turing::right);

    Turing::Turing_Machine<states, symbols, 100> machine(input, end, rules);

    for (auto item: machine.boot_up())
        std::cout << item;

}
