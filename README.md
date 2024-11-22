# Computational-Models

This is an ongoing project to create many different emultated computational systems and apply them to create more interesting computational systems.
The models created so far are:
 
## 1. Turing machine 

The Module Turing_Machine.ixx defines a basic turing machine model (one in which a tape starting from 0 has infinite length in one direction ~ a.k.a. a large pre-specified length for progarming purposes). The following projects extend this turing machine module to define more complicated systems.

### 1.1 Emulating a 4-bit RAM

Goal: To have a single tape turing machine emulate a 4-bit RAM and a simple instruction set that can be defined in memory and run.

The Module Emulating_4bit_RAM.ixx defines a 4-bit RAM that will run on the turing machine (Turing_Machine.ixx). Program_example.cpp contains an example program that runs the 4-bit RAM. The model contains a functioning random access memory with 4 registers that the LOAD and STORE commands can use. The model also contains a program counter (PC) that keeps track of where in memory code is being run from. The 4-bit machine "language" is defined as follows:
 
 Each command is given in 8 (2 by 4) bits:

  - first 2-Bits - Indicates command type 

    - 11 -> End program 

    - 01 -> Store

    - 10 -> load

    - 00 -> Unused (currently produces error)

  - second 2-bits - Indicates what register we are referring to.

  - The next 4 bits will indicate what memory location we are referring to. 

### 1.2 Emulating a Multitape turing machine
~Unfinished
## 2. Finite automata
The following projects aim to create modules that can create different types of finite automata that ate cabable of determining whether a given language is accepted.

### 2.1 Determenistic finite automata (DFA)

A deterministic finite automata is the simplest a

nd most intuitive model. The Deterministic Finite Automata.ixx module defines a DFA, and ExampleDFA.cpp provides an example of defining a DFA and querying its language acceptance.
