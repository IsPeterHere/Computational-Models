# Computational-Models

This is an ongoing project to create many different emulated computational systems and apply them to create more interesting computational systems.
The models created so far are:
 
## 1. Turing machine 

Turing_Machine.h defines a basic Turing machine model (one in which a tape starting from 0 has infinite length in one direction ~ a.k.a. a large pre-specified length for progarming purposes). The following projects extend this Turing machine module to define more complicated systems.

### 1.1 Emulating a 4-bit RAM

Goal: To have a single tape Turing machine emulate a 4-bit RAM and a simple instruction set that can be defined in memory and run.

Emulating_4bit_RAM.h defines a 4-bit RAM that will run on the Turing machine (Turing_Machine.h). Program_example.cpp contains an example program that runs the 4-bit RAM. The model contains a functioning random access memory with 4 registers that the LOAD and STORE commands can use. The model also contains a program counter (PC) that keeps track of where in memory code is being run from. The 4-bit machine "language" is defined as follows:
 
 Each command is given in 8 (2 by 4) bits:

  - first 2-Bits - Indicates command type 

    - 11 -> End program 

    - 01 -> Store

    - 10 -> load

    - 00 -> Unused (currently produces error)

  - second 2-bits - Indicates what register we are referring to.

  - The next 4 bits will indicate what memory location we are referring to. 

### 1.2 Emulating a Multitape Turing machine
~Unfinished
## 2. Finite automata
The following projects aim to create modules that can create different interfaces for finite automata that are capable of determining whether a given language is accepted.

### 2.1 Deterministic finite automata (DFA)

A deterministic finite automata is the most intuitive model of automata, in which states ate deterministically modified by the ordered input word. The Deterministic Finite Automata.h defines an interface for a DFA model, and ExampleDFA.cpp provides an example of defining a DFA and querying its language acceptance.

### 2.1 Non-Deterministic finite automata (NDFA)

A Non-deterministic finite automata is a fast and efficient generalization of a DFA, an equivalent DFA could could take up to 2^n states to simulate a NFA of n states. The non-Deterministic Finite Automata.h defines an interface for a NFA model, and ExampleNFA.cpp provides an example of defining a DFA and querying its language acceptance. The NFA uses sparse matrices to simulate the NFA resulting in fast querying of language acceptance even on NFAs with large numbers of nodes.
