/**
 * Project 1 
 * Assembler code fragment for LC-2K 
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXLINELENGTH 1000

int readAndParse(FILE *, char *, char *, char *, char *, char *);
int isNumber(char *);

int convertAndAdd(char*, int);

int main(int argc, char *argv[])
{
    char *inFileString, *outFileString;
    FILE *inFilePtr, *outFilePtr;
    char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH],
            arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];

    if (argc != 3) {
        printf("error: usage: %s <assembly-code-file> <machine-code-file>\n",
            argv[0]);
        exit(1);
    }
    inFileString = argv[1];
    outFileString = argv[2];

    inFilePtr = fopen(inFileString, "r");
    if (inFilePtr == NULL) {
        printf("error in opening %s\n", inFileString);
        exit(1);
    }
    outFilePtr = fopen(outFileString, "w");
    if (outFilePtr == NULL) {
        printf("error in opening %s\n", outFileString);
        exit(1);
    }

    int ads = 0; // Current address
    int max_label_num = 100;
    char labels[max_label_num][MAXLINELENGTH];
    // First pass
    while (readAndParse(inFilePtr,label,opcode,arg0,arg1,arg2))
    {
        // Check existence of duplicate label
        if (strlen(label)) // Label non-empty
        {
            char tmp[MAXLINELENGTH];
            strcpy(tmp,label);
            while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2))
            {
                if (!strcmp(tmp,label))
                    exit(1);
            }
            
            strcpy(labels[ads],tmp);

            rewind(inFilePtr);
            for (int i=0; i<=ads; i++)
                readAndParse(inFilePtr,label,opcode,arg0,arg1,arg2);
        }
        else
            strcpy(labels[ads],"-1");

        if (!strcmp(opcode, "add")) {}
        else if (!strcmp(opcode,"nor")) {}
        else if (!strcmp(opcode,"lw"))
        {
            if (isNumber(arg2))
            {
                int num_arg2 = atoi(arg2);
                if (num_arg2 < -32768 || num_arg2 > 32767)
                    exit(1);
            }
            else
            {
                char tmp[MAXLINELENGTH];
                strcpy(tmp,arg2);
                int tmp2 = 0;
                int flag = 0;
                rewind(inFilePtr);
                while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2))
                {
                    if (!strcmp(tmp,label))
                    {
                        if (tmp2 > 32767)
                            exit(1); // offsetFields don't fit in 16bits
                        flag = 1;
                        break;
                    }
                    tmp2 += 1;
                }
                if (!flag)
                    exit(1); // Using undefined label
                rewind(inFilePtr);
                for (int i=0; i<=ads; i++)
                    readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2);
            }
        }
        else if (!strcmp(opcode,"sw"))
        {
            if (isNumber(arg2))
            {
                int num_arg2 = atoi(arg2);
                if (num_arg2 < -32768 || num_arg2 > 32767)
                    exit(1);
            }
            else
            {
                char tmp[MAXLINELENGTH];
                strcpy(tmp,arg2);
                int tmp2 = 0;
                int flag = 0;
                rewind(inFilePtr);
                while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2))
                {
                    if (!strcmp(tmp,label))
                    {
                        if (tmp2 > 32767)
                            exit(1);
                        flag = 1;
                        break;
                    }
                    tmp2 += 1;
                }
                if (!flag)
                    exit(1);
                rewind(inFilePtr);
                for (int i=0; i<=ads; i++)
                    readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2);
            }
        }
        else if (!strcmp(opcode,"beq"))
        {
            if (isNumber(arg2))
            {
                int num_arg2 = atoi(arg2);
                if (num_arg2 < -32768 || num_arg2 > 32767)
                    exit(1);
            }
            else
            {
                char tmp[MAXLINELENGTH];
                strcpy(tmp,arg2);
                int tmp2 = 0;
                int flag = 0;
                rewind(inFilePtr);
                while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2))
                {
                    if (!strcmp(tmp,label))
                    {
                        int gap = tmp2 - ads;
                        if (gap < -32768 || gap > 32767)
                            exit(1);
                        flag = 1;
                        break;
                    }
                    tmp2 += 1;
                }
                if (!flag)
                    exit(1);
                rewind(inFilePtr);
                for (int i=0; i<=ads; i++)
                    readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2);
            }
        }
        else if (!strcmp(opcode,"jalr")) {}
        else if (!strcmp(opcode,"halt")) {}
        else if (!strcmp(opcode,"noop")) {}
        else if (!strcmp(opcode,".fill"))
        {
            if (!isNumber(arg0))
            {
                char tmp[MAXLINELENGTH];
                strcpy(tmp,arg0);
                //int tmp2 = 0;
                int flag = 0;
                rewind(inFilePtr);
                while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2))
                {
                    if (!strcmp(tmp,label))
                    {
                        flag = 1;
                        break;
                    }
                    //tmp2 += 1;
                }
                if (!flag)
                    exit(1);
                rewind(inFilePtr);
                for (int i=0; i<=ads; i++)
                    readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2);
            }
        }
        else
            exit(1); // Unrecognized opcodes
        
        ads++;  
    }

    int count = ads;
    rewind(inFilePtr);
    ads = 0;
    // Second pass
    while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2))
    {
        int result = 0;
        if (!strcmp(opcode, "add")) 
        {
            result += convertAndAdd(arg0,19);
            result += convertAndAdd(arg1,16);
            result += convertAndAdd(arg2,0);
        }
        else if (!strcmp(opcode,"nor"))
        {
            result += (1<<22);
            result += convertAndAdd(arg0,19);
            result += convertAndAdd(arg1,16);
            result += convertAndAdd(arg2,0);
        }
        else if (!strcmp(opcode,"lw"))
        {
            result += (1<<23);
            result += convertAndAdd(arg0,19);
            result += convertAndAdd(arg1,16);
            if (isNumber(arg2))
            {
                int num_arg2 = atoi(arg2);
                if (num_arg2 >= -32768 && num_arg2 < 0)
                    result += ((1<<16) + num_arg2);
                else if (num_arg2 >= 0 && num_arg2 <= 32767)
                    result += num_arg2;
            }
            else
            {   
                for (int i=0; i<count; i++)
                {
                    if (!strcmp(arg2,labels[i]))
                    {
                        result += i;
                        break;
                    }
                }
            }
        }
        else if (!strcmp(opcode,"sw"))
        {
            result += ((1<<23)+(1<<22));
            result += convertAndAdd(arg0,19);
            result += convertAndAdd(arg1,16);
            if (isNumber(arg2))
            {
                int num_arg2 = atoi(arg2);
                if (num_arg2 >= -32768 && num_arg2 < 0)
                    result += ((1<<16) + num_arg2);
                else if (num_arg2 >= 0 && num_arg2 <= 32767)
                    result += num_arg2;
            }
            else
            {
                for (int i=0; i<count; i++)
                {
                    if (!strcmp(arg2,labels[i]))
                    {
                        result += i;
                        break;
                    }
                }
            }
        }
        else if (!strcmp(opcode,"beq"))
        {
            result += (1<<24);
            result += convertAndAdd(arg0,19);
            result += convertAndAdd(arg1,16);
            if (isNumber(arg2))
            {
                int num_arg2 = atoi(arg2);
                if (num_arg2 >= -32768 && num_arg2 < 0)
                    result += ((1<<16) + num_arg2);
                else if (num_arg2 >= 0 && num_arg2 <= 32767)
                    result += num_arg2;
            }
            else
            {   
                for (int i=0; i<count; i++)
                {
                    if (!strcmp(arg2,labels[i]))
                    {
                        int gap = i - ads -1;
                        if (gap < 0)
                            result += ((1<<16)+gap);
                        else
                            result += gap;
                        break;
                    }
                }
            }
        }
        else if (!strcmp(opcode,"jalr"))
        {
            result += ((1<<24)+(1<<22));
            result += convertAndAdd(arg0,19);
            result += convertAndAdd(arg1,16);
        }
        else if (!strcmp(opcode,"halt"))
        {
            result = (1<<23)+(1<<24);
        }
        else if (!strcmp(opcode,"noop"))
        {
            result = (1<<22)+(1<<23)+(1<<24);
        }
        else if (!strcmp(opcode,".fill"))
        {
            if (isNumber(arg0))
                result = atoi(arg0);
            else
            {
                for (int i=0; i<count; i++)
                {
                    if (!strcmp(labels[i],arg0))
                    {
                        result = i;
                        break;
                    }
                }
            }
        }
        
        ads++;
        fprintf(outFilePtr,"%d\n",result);
    }
    fclose(inFilePtr);
    fclose(outFilePtr);
    
    exit(0);
}

/*
 * Read and parse a line of the assembly-language file.  Fields are returned
 * in label, opcode, arg0, arg1, arg2 (these strings must have memory already
 * allocated to them).
 *
 * Return values:
 *     0 if reached end of file
 *     1 if all went well
 *
 * exit(1) if line is too long.
 */
int readAndParse(FILE *inFilePtr, char *label, char *opcode, char *arg0, char *arg1, char *arg2)
{
    char line[MAXLINELENGTH];
    char *ptr = line;

    /* delete prior values */
    label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';

    /* read the line from the assembly-language file */
    if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL) {
	/* reached end of file */
        return 0;
    }

    /* check for line too long */
    if (strlen(line) == MAXLINELENGTH-1) {
	printf("error: line too long\n");
	exit(1);
    }

    /* is there a label? */
    ptr = line;
    if (sscanf(ptr, "%[^\t\n ]", label)) {
	/* successfully read label; advance pointer over the label */
        ptr += strlen(label);
    }

    /*
     * Parse the rest of the line.  Would be nice to have real regular
     * expressions, but scanf will suffice.
     */
    sscanf(ptr, "%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]",
        opcode, arg0, arg1, arg2);
    return 1;
}

int isNumber(char *string)
{
    /* return 1 if string is a number */
    int i;
    return( (sscanf(string, "%d", &i)) == 1);
}


int convertAndAdd(char* arg, int power)
{
    int result = 0;
    int tmp = power;
    int num_arg0 = atoi(arg);
    while (num_arg0)
    {
        int remainder = num_arg0 % 2;
        if (remainder)
            result += (1 << tmp);
        num_arg0 /= 2;
        tmp += 1;
    }
    return result;
}