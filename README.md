# Excalibur

Excalibur is a Turing machine framework with multiple functions, relating to the research of 6-state 2-symbol Turing machines. Its forward simulator is capable of computing approximately 250,000,000 steps per second, but this may vary significantly. The goal of this project is not to create the fastest simulator, however. As this is a work-in-progress, the functionality of this project is rather limited. Eventually, the goal is for Excalibur to specilize in backward reasoning, and to expand on the current mathematical framework of it. One particularly complex Turing machine is Antihydra, which is elusive to all current variants of backward reasoning, but has been and will be a center of attention for this project.

At the current moment, Excalibur is capable of only forward simulation of 6-state, 2-symbol Turing machines. While it is not designed to be competitively fast, efforts to optimize it are still significant; a lot of time was spent making the forward simulator faster.

## Turing Machines and Beavers
In the sense of Excalibur, a Turing machine consists of a state, a signed index, a set of instructions, and a sequence of bits. We say that the index denotes the position of the machine's head. At each step, the machine reads the bit at the index, considers the current state, and fetches the corresponding instruction. This instruction tells it to rewrite that bit as a certain boolean, either increment or decrement the head, and switch to a new state. In the broader field of theoretical computer science, a Turing machine is a very real concept, and this is my implementation of it. The Busy Beaver function, given some natural `n`, returns the runtime of the longest running Turing machine that halts with 2 symbols and `n` states. The computation of the sixth value of that function is the goal of this project.

## Future Work
In the short-term future, the goal is to create a basic backward reasoning simulator. It will likely be more complicated than forward simulation. Beyond that, more advanced frameworks which build on basic BR would come into question. There is no precedent at the current moment for implementing these frameworks into code. As far as the author is aware, previous attempts to implement them have failed. There is question as to whether or not these are even feasible to commit to code.

## Usage Guide
1. Run the compiled binary
2. When prompted, input a valid 6-state, 2-symbol Turing Machine

For example, paste any of the following Turing Machines:
```
1RB1RA_0LC1LE_1LD1LC_1LA0LB_1LF1RE_---0RA
1RB0RC_0RC1RC_0LD0RA_1LE0LF_0LA1LC_---1LA
1RB0RE_1LC1RA_1LD0LC_1LA0LA_1RA1RF_0RA---
1RB0LE_1LC0RD_0RA1LD_0LC1RA_1LF1LA_---1LA
```

The machine will be simulated for 1,000,000,000 steps, or until it halts. The total runtime is printed after the simulation is complete.