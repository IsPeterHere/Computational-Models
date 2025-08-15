# Computational-Models

This is an ongoing project to create many different emulated computational systems and apply them to create more interesting computational systems.
The models created and defined in ComM.h so far are:
 
## 1. Turing machine 

A basic Turing machine model (Specifically as descried in 1.) is defined, only limited with an adjustable predetermined bound on the tape length for implementation purposes.

sources:  
1. Papadimitriou, Christos H . Computational Complexity. United Kingdom, Addison-Wesley, 1993.

 Where this model has been used:   
- <a href = "https://github.com/IsPeterHere/4-Bit-RAM">A minimalistic 4-bit-RAM architecture running on a single tape Turing machine</a>


## 2. Finite automata
The following are different finite automata that are capable, once set-up of determining whether a given word is accepted.

### 2.1 Non-Deterministic finite automata (NFA)

A Non-deterministic finite automata is a fast and efficient generalization of a DFA, an equivalent DFA could could take up to 2^n states to simulate a NFA of n states. The non-Deterministic Finite Automata.h defines an interface for a NFA model, and ExampleNFA.cpp provides an example of defining a DFA and querying its language acceptance. The NFA uses sparse matrices to simulate the NFA resulting in fast querying of language acceptance even on NFAs with large numbers of nodes.

sources:  
1.  Jewls of Formal Language Theory ch2 L2.1 ~ A.Salommaa  

### 2.2 Alternating Finite Automata (r-AFA)

Alternating finite automata are a generalized method for dealing with non-deterministic automata. An r-AFA can have its transition functioned defined by a series of regular expressions leading to a more *programmable* Automata. Any n-state DFA can be represented by an equivalent r-AFA of at most log(n) +1 states. This implementation uses Boolean vectors to store and efficiently evaluate the transition function giving tremendous space and speed savings in comparison to a symbolic simulation.

sources:  
1.  Handbook of Formal Languages vol1 2.3.7 ~ G.rozenberg & A.salomaa  


## 2. L-Systems

### 2.1 Deterministic context-free L-System (DOL System)
 A Basic DOL-system that can efficiently perform substitutions on a given word iteratively.

 sources:  
1.  Jewls of Formal Language Theory ch2 L2.1 ~ A.Salommaa

2.  Where this model has been used:   
- <a href = "https://github.com/IsPeterHere/L-System-Sunlight-Competition">L-System Sunlight Competition</a>
