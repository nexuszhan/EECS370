This repo is a backup of UMich EECS370 (Introdution to Computer Architecture) projects. Students from UMich should obey the Honor Code when accessing this repo. 

Project1 
assembler: a C program which takes as input an LC2K assembly file (denoted with *.as) and outputs its correct machine code representation into a machine code file (denoted with *.mc)
simulator: a C program which simulates the LC2K ISA, with a given machine code file as input. It will output the simulation to stdout.
multiplication: an LC2K assembly program which multiplies two positive 15 bit numbers.

Project3 implements a simulator that takes in a machine code file (reading and parsing instructions has been taken care by the starter code), and execute the machine code instructions in the way a pipeline processor would.

Project4 simulates a CPU cache (unified instruction/data) and integrate the cache into a Project 1 (behavioral) simulator. As the processor simulator executes an assembly-language program, it will access instructions and data. These accesses will be serviced by the cache, which will transfer data to/from memory as needed.