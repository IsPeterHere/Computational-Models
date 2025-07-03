# Computational-Models

This is an ongoing project to create many different emulated computational systems and apply them to create more interesting computational systems.
The models created so far are:
 
## 1. Turing machine 

Turing_Machine.h defines a basic Turing machine model (one in which a tape starting from 0 has infinite length in one direction ~ a.k.a. a large pre-specified length for programming purposes). The following projects extend this Turing machine module to define more complicated systems.

### Where this model has been used:
<a href = "https://github.com/IsPeterHere/4-Bit-RAM">A minimalistic 4-bit-RAM architecture running on a single tape Turing machine</a>


## 2. Finite automata
The following projects aim to create modules that can create different interfaces for finite automata that are capable of determining whether a given word is accepted (And then witch is hence part of the language accepted by such an automata).

### 2.1 Non-Deterministic finite automata (NFA)

A Non-deterministic finite automata is a fast and efficient generalization of a DFA, an equivalent DFA could could take up to 2^n states to simulate a NFA of n states. The non-Deterministic Finite Automata.h defines an interface for a NFA model, and ExampleNFA.cpp provides an example of defining a DFA and querying its language acceptance. The NFA uses sparse matrices to simulate the NFA resulting in fast querying of language acceptance even on NFAs with large numbers of nodes.

sources:  
Jewls of Formal Language Theory ch2 L2.1 ~ A.Salommaa  

### 2.2 Alternating Finite Automata (r-AFA)

Alternating finite automata are a generalized method for dealing with non-deterministic automata. An r-AFA can have its transition functioned defined by a series of regular expressions leading to a more *programmable* Automata. Any n-state DFA can be represented by an equivalent r-AFA of at most log(n) +1 states. This implementation uses Boolean vectors to store and efficiently evaluate the transition function giving tremendous space and speed savings in comparison to a symbolic simulation.

sources:  
Handbook of Formal Languages vol1 2.3.7 ~ G.rozenberg & A.salomaa  
