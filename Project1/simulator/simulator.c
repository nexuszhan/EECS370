/*
 * Project 1
 * EECS 370 LC-2K Instruction-level simulator
 *
 * Make sure *not* to modify printState or any of the associated functions
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Machine Definitions
#define NUMMEMORY 65536 /* maximum number of words in memory */
#define NUMREGS 8 /* number of machine registers */

// File
#define MAXLINELENGTH 1000 /* MAXLINELENGTH is the max number of characters we read */

typedef struct stateStruct {
    int pc;
    int mem[NUMMEMORY];
    int reg[NUMREGS];
    int numMemory;
} stateType;

void printState(stateType *);

int convertNum(int);

int main(int argc, char *argv[])
{
    char line[MAXLINELENGTH];
    stateType state;
    FILE *filePtr;

    if (argc != 2) {
        printf("error: usage: %s <machine-code file>\n", argv[0]);
        exit(1);
    }

    filePtr = fopen(argv[1], "r");
    if (filePtr == NULL) {
        printf("error: can't open file %s", argv[1]);
        perror("fopen");
        exit(1);
    }

    /* read the entire machine-code file into memory */
    for (state.numMemory = 0; fgets(line, MAXLINELENGTH, filePtr) != NULL; state.numMemory++) {
        if (sscanf(line, "%d", state.mem+state.numMemory) != 1) {
            printf("error in reading address %d\n", state.numMemory);
            exit(1);
        }
        printf("memory[%d]=%d\n", state.numMemory, state.mem[state.numMemory]);
    }

    //Your code starts here
    state.pc = 0;
    for (int i=0; i<NUMREGS; i++)
        state.reg[i] = 0;
    int instr = 0;
    while (1)
    {
        printState(&state);
        instr++;

        int mc = state.mem[state.pc];
        int opcode = mc >> 22;
        //printf("%d ",opcode);
        if (opcode < 6)
        {
            int regA = (mc & 3670016) >> 19;
            int regB = (mc & 458752) >> 16;
            //printf("%d %d\n",regA,regB);
            if (opcode == 0)
            {
                int destReg = mc & 65535;
                state.reg[destReg] = state.reg[regA] + state.reg[regB];
            }
            else if (opcode == 1)
            {
                int destReg = mc & 65535;
                state.reg[destReg] = ~(state.reg[regA] | state.reg[regB]);
            }
            else if (opcode == 2)
            {
                int offsetField = convertNum(mc & 65535);
                int address = state.reg[regA] + offsetField;
                state.reg[regB] = state.mem[address];
            }
            else if (opcode == 3)
            {
                int offsetField = convertNum(mc & 65535);
                int address = state.reg[regA] + offsetField;
                state.mem[address] = state.reg[regB];
            }
            else if (opcode == 4)
            {
                int offsetField = convertNum(mc & 65535);
                if (state.reg[regA] == state.reg[regB])
                    state.pc += offsetField;
            }
            else if (opcode == 5)
            {
                state.reg[regB] = state.pc + 1;
                state.pc = state.reg[regA];
                continue;
            }
        }
        else if (opcode == 6)
        {
            state.pc++;
            break;
        }

        state.pc++;
    }

    printf("machine halted\ntotal of %d instructions executed\nfinal state of machine:\n",instr);
    printState(&state);
    
    return(0);
}

void printState(stateType *statePtr)
{
    int i;
    printf("\n@@@\nstate:\n");
    printf("\tpc %d\n", statePtr->pc);
    printf("\tmemory:\n");
    for (i=0; i<statePtr->numMemory; i++) {
              printf("\t\tmem[ %d ] %d\n", i, statePtr->mem[i]);
    }
    printf("\tregisters:\n");
    for (i=0; i<NUMREGS; i++) {
              printf("\t\treg[ %d ] %d\n", i, statePtr->reg[i]);
    }
    printf("end state\n");
}

int convertNum(int num)
{
    /* convert a 16-bit number into a 32-bit Linux integer */
    if (num & (1<<15) ) {
        num -= (1<<16);
    }
    return(num);
}

