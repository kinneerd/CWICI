#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "instructions.h"
#include "table.h"
#include "stack.h"

#define INSTR_TABLE_SIZE 100

typedef struct {
    char opcode[6];
    char operand[21];
} instructionType;

typedef struct
{
    instructionType instructions[INSTR_TABLE_SIZE];
    int instrCount;
} instructionTable;

// These 4 are "private" making them only accessible via public function
static instructionTable instrTable;
static tableType symbolTable;   //for variables and values
static tableType jumpTable;     //for labels
static stackType stack;         //for stack operations 

// This one is public so main can use it.
void initialize()
{
   instrTable.instrCount = 0; 
   initializeTable(&symbolTable);
   initializeTable(&jumpTable);
   initializeStack(&stack);
}

void printSymbolTable()
{
    printTable(&symbolTable);
}

void printInstructionTable(){
    int i;
    for(i = 0; i < instrTable.instrCount; i++){
        if(hasOperand(instrTable.instructions[i].opcode))
            printf("%s %s\n", instrTable.instructions[i].opcode, instrTable.instructions[i].operand);
        else
            printf("%s\n", instrTable.instructions[i].opcode);
        printf("\n");
    }
}

void printTables(){
    printf("\n-- Instruction Table --\n\n");
    printInstructionTable();

    printf("\n-- Symbol Table --\n\n");
    printTable(&symbolTable);

    printf("\n-- Jump Table --\n\n");
    printTable(&jumpTable);

    printf("\n-- Stack --\n\n");
    printStack(&stack);
}

int fetchInstruction(int address, char * opcode, char * operand)
{
    if(address <= instrTable.instrCount){
        memcpy(opcode, instrTable.instructions[address].opcode, OPCODE_SIZE +1);
        memcpy(operand, instrTable.instructions[address].operand, OPERAND_SIZE +1);
    } else{
        printf("Fetched invalid address: '%d', error.\n", address);
        exit(1);
    }
    return 1;
}

void insertInstruction(int address, char * opcode, char * operand)
{
    if(address <= instrTable.instrCount){
        if(strcmp(operand, "label") == 0){
            strcpy(instrTable.instructions[address].opcode, "nop");
            store(&jumpTable, opcode, address);
        } else{
            strcpy(instrTable.instructions[address].opcode, opcode);
            strcpy(instrTable.instructions[address].operand, operand);
        } 
        instrTable.instrCount++;
    } else {
        printf("Invalid address ('%d') inserted, error.\n", address);
        exit(1);
    }
}

int hasOperand(char * opcode)
{
    if(strcmp(opcode, "get") == 0){return 1;}
    else if(strcmp(opcode, "put") == 0){return 1;}
    else if(strcmp(opcode, "push") == 0){return 1;}
    else if(strcmp(opcode, "pop") == 0){return 1;}
    else if(strcmp(opcode, "j") == 0){return 1;}
    else if(strcmp(opcode, "jf") == 0){return 1;}

    else if (strcmp(opcode, "nop") == 0){return 0;}
    else if (strcmp(opcode, "add") == 0){return 0;}
    else if (strcmp(opcode, "sub") == 0){return 0;}
    else if (strcmp(opcode, "mul") == 0){return 0;}
    else if (strcmp(opcode, "div") == 0){return 0;}
    else if (strcmp(opcode, "and") == 0){return 0;}
    else if (strcmp(opcode, "not") == 0){return 0;}
    else if (strcmp(opcode, "or") == 0){return 0;}
    else if (strcmp(opcode, "tsteq") == 0){return 0;}
    else if (strcmp(opcode, "tstne") == 0){return 0;}
    else if (strcmp(opcode, "tstlt") == 0){return 0;}
    else if (strcmp(opcode, "tstle") == 0){return 0;}
    else if (strcmp(opcode, "tstgt") == 0){return 0;}
    else if (strcmp(opcode, "tstge") == 0){return 0;}
    else if (strcmp(opcode, "halt") == 0){return 0;}

    else { return 1; } // must be a label!!
}

//**********************************************************************
// Begin opcode function section.
//**********************************************************************
//Functions for various instructions go here. 
int nop(int pc) { return pc + 1; }

int add(int pc){
    int rop = stackPop(&stack);
    int lop = stackPop(&stack);
    stackPush(&stack, lop + rop);
    return pc + 1;
}

int sub(int pc){
    int rop = stackPop(&stack);
    int lop = stackPop(&stack);
    stackPush(&stack, lop - rop);
    return pc + 1;
}

int mul(int pc){
    int rop = stackPop(&stack);
    int lop = stackPop(&stack);
    stackPush(&stack, lop * rop);
    return pc + 1;
}

int divide(int pc){
    int rop = stackPop(&stack);
    int lop = stackPop(&stack);
    if(rop == 0){
        printf("\nDivide by '0' error, bye.");
        exit(1);
    }
    stackPush(&stack, lop / rop);
    return pc + 1;
}

int get(int pc, char * operand){
    int user;
    char numstring[10];
    char *endptr;
    
    printf("enter %s > ", operand);
    fgets(numstring, 10, stdin);
    user = strtol(numstring, &endptr, 10);
    
    if(*endptr != '\n'){
        while(*endptr != '\n'){
            endptr++;
        }
        get(pc, operand);
    } else{
        store(&symbolTable, operand, user);
    }
    return pc + 1;
}

int put(int pc, char * operand){
    int tmp = retrieve(&symbolTable, operand);
    printf("%d\n", tmp);
    return pc + 1;
}

int push(int pc, char * operand){ 
    int tmp;
    if(isdigit(*operand))
        tmp = atoi(operand);
    else
        tmp = retrieve(&symbolTable, operand);
    stackPush(&stack, tmp);
    return pc + 1;
}

int pop(int pc, char * operand){
    int var = stackPop(&stack);
    store(&symbolTable, operand, var);
    return pc + 1;
}

int not(int pc){
    int x = stackPop(&stack);
    stackPush(&stack, !x);
    return pc + 1;
}

int and(int pc){
    int rop = stackPop(&stack);
    int lop = stackPop(&stack);
    stackPush(&stack, lop && rop);
    return pc + 1;
}

int or(int pc){
    int rop = stackPop(&stack);
    int lop = stackPop(&stack);
    stackPush(&stack, lop || rop);
    return pc + 1;
}

int testeq(int pc){
    int x = stackPop(&stack);
    stackPush(&stack, x == 0);
    return pc + 1;
}

int testne(int pc){
    int x = stackPop(&stack);
    stackPush(&stack, x != 0);
    return pc + 1;
}

int testlt(int pc){
    int x = stackPop(&stack);
    stackPush(&stack, x < 0);
    return pc + 1;
}

int testle(int pc){
    int x = stackPop(&stack);
    stackPush(&stack, x <= 0);
    return pc + 1;
}

int testgt(int pc){
    int x = stackPop(&stack);
    stackPush(&stack, x > 0);
    return pc + 1;
}

int testge(int pc){
    int x = stackPop(&stack);
    stackPush(&stack, x >= 0);
    return pc + 1;
}

int jump(int pc, char * operand){
    int j_pc = retrieve(&jumpTable, operand);
    return j_pc;
}

int jf(int pc, char * operand){
    if(stackPop(&stack) == 1)
        return pc + 1;
    else
        return retrieve(&jumpTable, operand);
}

int halt(int pc){
    return pc + 1;
}
