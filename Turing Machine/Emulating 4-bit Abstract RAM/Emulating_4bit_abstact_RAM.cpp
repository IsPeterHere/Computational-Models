#include <iostream>
#include <vector>
#include <Turing.h>


enum states
{
    //Core Commands
    go,
    end,
    reset,
    get,
    read_Q,

    //Adjustments
    
    last_LOC,

    //4-bit data segments (In sequentail blocks of Construction)
    Q1XXX,
    Q0XXX,

    Q10XX,
    Q01XX,
    Q11XX,
    Q00XX,

    Q111X,
    Q110X,
    Q101X,
    Q100X,
    Q011X,
    Q010X,
    Q001X,
    Q000X,

    Q1110,
    Q1100,
    Q1010,
    Q1000,
    Q0110,
    Q0100,
    Q0010,
    Q0000,
    Q1111,
    Q1101,
    Q1011,
    Q1001,
    Q0111,
    Q0101,
    Q0011,
    Q0001,
    

    //4-bit write Commands Left

    L1XXX,
    L0XXX,

    L10XX,
    L01XX,
    L11XX,
    L00XX,

    L111X,
    L110X,
    L101X,
    L100X,
    L011X,
    L010X,
    L001X,
    L000X,

    L1110,
    L1100,
    L1010,
    L1000,
    L0110,
    L0100,
    L0010,
    L0000,
    L1111,
    L1101,
    L1011,
    L1001,
    L0111,
    L0101,
    L0011,
    L0001,


    
};

enum symbols
{
    //Base Markers
    Zero,
    One,
    Start,
    Blank,

    PC_REG,

    //Memory Mangment
    LOC,
    SEND_BACK,
#

    //Core Instructions
    Reset,
    Get_command,
    Get_memory,

};



// 4-bit Language Specification ~ as a 4 bit language it is fairly trivial
// Each comand is given in 2*4 bits
// first Bit - Indicates what comand is this (i.e Load, Store,end,goto)
// second bit - ...^
// thrid bit - Indicates what register we are refering to
// fourth bit - ...^
// The Next 4 bits will indicate what memory location we are refering to 
int main()
{
    Turing::Rules<states, symbols> rules{};

    //DEFINE GROUPS 

    std::vector<states> Q_Segments_full
    {
        Q1110,Q1100,Q1010,Q1000,Q0110,Q0100,Q0010,Q0000,Q1111,Q1101,Q1011,Q1001,Q0111,Q0101,Q0011,Q0001,
    };

    std::vector<states> L_Write_Segments_full
    {
        L1110,L1100,L1010,L1000,L0110,L0100,L0010,L0000,L1111,L1101,L1011,L1001,L0111,L0101,L0011,L0001,
    };

    //CORE COMMANDS
    rules.add({ go, Start }, { go, Start , Turing::right });
    rules.add({ go, Zero }, { go, Zero , Turing::right });
    rules.add({ go, One }, { go, One , Turing::right });

    rules.add({ go, Reset }, { reset, Get_command , Turing::left }); // reset the Core command and memory registers
    rules.add({ reset, Zero }, { reset, Blank , Turing::left });
    rules.add({ reset, One }, { reset, Blank , Turing::left });
    rules.add({ reset, Start }, { go, Start , Turing::right }); // Until start

    rules.add({ go, Get_command }, { get, Get_memory, Turing::right }); // Fill Core command register
    rules.add({ get, Zero }, { get, Zero , Turing::right });
    rules.add({ get, One }, { get, One , Turing::right });
    rules.add({ get, PC_REG }, { read_Q, PC_REG , Turing::right });

    rules.add({ go, Get_memory }, { end, Get_memory, Turing::right });


    //Define Q-Read ~ read 4 bits goto location specifeid, read 4 bits & then return 4 bits
    rules.add({ read_Q, One }, { Q1XXX, One , Turing::right });
    rules.add({ read_Q, Zero },{ Q0XXX, Zero , Turing::right });

    rules.add({ Q1XXX, One },  { Q11XX, One , Turing::right });
    rules.add({ Q1XXX, Zero }, { Q10XX, Zero , Turing::right });
    rules.add({ Q0XXX, One },  { Q01XX, One , Turing::right });
    rules.add({ Q0XXX, Zero }, { Q00XX, Zero , Turing::right });


    rules.add({ Q11XX, One },  { Q111X, One , Turing::right });
    rules.add({ Q11XX, Zero }, { Q110X, Zero , Turing::right });
    rules.add({ Q10XX, One },  { Q101X, One , Turing::right });
    rules.add({ Q10XX, Zero }, { Q100X, Zero , Turing::right });
    rules.add({ Q01XX, One },  { Q011X, One , Turing::right });
    rules.add({ Q01XX, Zero }, { Q010X, Zero , Turing::right });
    rules.add({ Q00XX, One },  { Q001X, One , Turing::right });
    rules.add({ Q00XX, Zero }, { Q000X, Zero , Turing::right });

    rules.add({ Q111X, One },  { Q1111, One , Turing::right });
    rules.add({ Q111X, Zero }, { Q1110, Zero , Turing::right });
    rules.add({ Q110X, One },  { Q1101, One , Turing::right });
    rules.add({ Q110X, Zero }, { Q1100, Zero , Turing::right });
    rules.add({ Q101X, One },  { Q1011, One , Turing::right });
    rules.add({ Q101X, Zero }, { Q1010, Zero , Turing::right });
    rules.add({ Q100X, One },  { Q1001, One , Turing::right });
    rules.add({ Q100X, Zero }, { Q1000, Zero , Turing::right });
    rules.add({ Q011X, One },  { Q0111, One , Turing::right });
    rules.add({ Q011X, Zero }, { Q0110, Zero , Turing::right });
    rules.add({ Q010X, One },  { Q0101, One , Turing::right });
    rules.add({ Q010X, Zero }, { Q0100, Zero , Turing::right });
    rules.add({ Q001X, One },  { Q0011, One , Turing::right });
    rules.add({ Q001X, Zero }, { Q0010, Zero , Turing::right });
    rules.add({ Q000X, One },  { Q0001, One , Turing::right });
    rules.add({ Q000X, Zero }, { Q0000, Zero , Turing::right });

    rules.add(Q_Segments_full, Zero , Q_Segments_full, Zero , Turing::right ); //Skip 
    rules.add(Q_Segments_full, One ,  Q_Segments_full, One , Turing::right );

    rules.add({ Q1111, LOC }, { Q1110, LOC , Turing::right }); //count down
    rules.add({ Q1110, LOC }, { Q1101, LOC , Turing::right });
    rules.add({ Q1101, LOC }, { Q1100, LOC , Turing::right });
    rules.add({ Q1100, LOC }, { Q1011, LOC , Turing::right });
    rules.add({ Q1011, LOC }, { Q1010, LOC , Turing::right });
    rules.add({ Q1010, LOC }, { Q1001, LOC , Turing::right });
    rules.add({ Q1001, LOC }, { Q1000, LOC , Turing::right });
    rules.add({ Q1000, LOC }, { Q0111, LOC , Turing::right });
    rules.add({ Q0111, LOC }, { Q0110, LOC , Turing::right });
    rules.add({ Q0110, LOC }, { Q0101, LOC , Turing::right });
    rules.add({ Q0101, LOC }, { Q0100, LOC , Turing::right });
    rules.add({ Q0100, LOC }, { Q0011, LOC , Turing::right });
    rules.add({ Q0011, LOC }, { Q0010, LOC , Turing::right });
    rules.add({ Q0010, LOC }, { Q0001, LOC , Turing::right });
    rules.add({ Q0001, LOC }, { Q0000, LOC , Turing::right });

    rules.add({ Q0000, LOC }, { last_LOC, SEND_BACK , Turing::left }); //reached destination

    rules.add({ last_LOC, Zero }, { last_LOC, Zero , Turing::left }); //Skip back to last LOC (or PC)
    rules.add({ last_LOC, One }, { last_LOC, One , Turing::left });
    rules.add({ last_LOC, LOC }, { read_Q, LOC , Turing::right }); //Read contents
    rules.add({ last_LOC, PC_REG }, { read_Q, PC_REG , Turing::right }); //Read contents
    rules.add(Q_Segments_full, SEND_BACK, L_Write_Segments_full, LOC , Turing::left); // Now return and write


    //L_Write ~ Move left until blank space is found then write 
    rules.add( L_Write_Segments_full, Zero, L_Write_Segments_full, Zero , Turing::left ); //Skip back Until a Blank is found
    rules.add( L_Write_Segments_full, One , L_Write_Segments_full, One , Turing::left );
    rules.add( L_Write_Segments_full, LOC , L_Write_Segments_full, LOC , Turing::left );
    rules.add(L_Write_Segments_full, PC_REG, L_Write_Segments_full, PC_REG, Turing::left);
    rules.add(L_Write_Segments_full, Get_command, L_Write_Segments_full, Get_command, Turing::left);

    rules.add({ Q1111, Blank }, { Q111X, One , Turing::left });//Write
    rules.add({ Q1110, Blank }, { Q111X, Zero , Turing::left });
    rules.add({ Q1101, Blank }, { Q110X, One , Turing::left });
    rules.add({ Q1100, Blank }, { Q110X, Zero , Turing::left });
    rules.add({ Q1011, Blank }, { Q101X, One , Turing::left });
    rules.add({ Q1010, Blank }, { Q101X, Zero , Turing::left });
    rules.add({ Q1001, Blank }, { Q100X, One , Turing::left });
    rules.add({ Q1000, Blank }, { Q100X, Zero , Turing::left });
    rules.add({ Q0111, Blank }, { Q011X, One , Turing::left });
    rules.add({ Q0110, Blank }, { Q011X, Zero , Turing::left });
    rules.add({ Q0101, Blank }, { Q010X, One , Turing::left });
    rules.add({ Q0100, Blank }, { Q010X, Zero , Turing::left });
    rules.add({ Q0011, Blank }, { Q001X, One , Turing::left });
    rules.add({ Q0010, Blank }, { Q001X, Zero , Turing::left });
    rules.add({ Q0001, Blank }, { Q000X, One , Turing::left });
    rules.add({ Q0000, Blank }, { Q000X, Zero , Turing::left });

    rules.add({ Q111X, Blank }, { Q11XX, One , Turing::left });
    rules.add({ Q110X, Blank }, { Q11XX, Zero , Turing::left });
    rules.add({ Q101X, Blank }, { Q10XX, One , Turing::left });
    rules.add({ Q100X, Blank }, { Q10XX, Zero , Turing::left });
    rules.add({ Q011X, Blank }, { Q01XX, One , Turing::left });
    rules.add({ Q010X, Blank }, { Q01XX, Zero , Turing::left });
    rules.add({ Q001X, Blank }, { Q00XX, One , Turing::left });
    rules.add({ Q000X, Blank }, { Q00XX, Zero , Turing::left });

    rules.add({ Q11XX, Blank }, { Q1XXX, One , Turing::left });
    rules.add({ Q10XX, Blank }, { Q1XXX, Zero , Turing::left });
    rules.add({ Q01XX, Blank }, { Q0XXX, One , Turing::left });
    rules.add({ Q00XX, Blank }, { Q0XXX, Zero , Turing::left });

    rules.add({ Q1XXX, Blank }, { go, One , Turing::left });
    rules.add({ Q0XXX, Blank }, { go, Zero , Turing::left }); //now go and Get_memory


   



    Turing::Turing_Machine<states, symbols, 100> machine(end, rules); //Create a Machine with 100 tape segments that will end when state "end" is reached and that uses our rule set



    std::vector<symbols> input
    {
        //Core
        Start,
        Zero,Zero,Zero,Zero, //Memory register    
        Zero,Zero,Zero,Zero, //Comand register
        Reset,
        //Registers
        Zero,Zero,Zero,Zero, //r1
        Zero,Zero,Zero,Zero, //r2
        Zero,Zero,Zero,Zero, //r3
        Zero,Zero,Zero,Zero, //r4

        PC_REG,//Program Counter is in #0000
        Zero,Zero,Zero,Zero, //#0000
        LOC,
        Zero,Zero,Zero,Zero, //#0001
        LOC,
        One,One,One,Zero,    //#0010
        LOC,
        Zero,Zero,Zero,Zero, //#0011
        LOC,
        Zero,Zero,Zero,Zero, //#0100
        LOC,
        Zero,Zero,Zero,Zero, //#0101
        LOC,
        Zero,Zero,Zero,Zero, //#0110
        LOC,
        Zero,Zero,Zero,Zero, //#0111
        LOC,
        Zero,Zero,Zero,Zero, //#1000
        LOC,
        Zero,Zero,Zero,Zero, //#1001
        LOC,
        Zero,Zero,Zero,Zero, //#1010
        LOC,
        Zero,Zero,Zero,Zero, //#1011
        LOC,
        Zero,Zero,Zero,Zero, //#1100
        LOC,
        Zero,Zero,Zero,Zero, //#1101
        LOC,
        Zero,Zero,Zero,Zero, //#1110
        LOC,
        Zero,Zero,Zero,Zero, //#1111
        LOC,

    };


    for (auto item : machine.boot_up(input)) // On the Machine halting the contents of the tape left of the head is outputted
        std::cout << item;

}