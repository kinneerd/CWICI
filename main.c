#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdarg.h>

#include "instructions.h"
#include "stack.h"
#include "table.h"

// "Private" methods to break up our work here.
static void readInstructions(FILE * fp);
static void discardline(FILE * fp);
static void execute();

int main(int argc, char * argv[]){
    //First things first. Open the input file.
    FILE *fp;
    int i = 0;
        
    if (argc != 2 || (fp = fopen(argv[1], "r")) == NULL) {
      printf("File open failed.\nUsage: %s <input file>\n",argv[0]);
      exit(1);
    }

    // initialize the symbol table, jump table and stack to 0
    // method will be in instructions.c
    initialize();

    // read the input file and prepare structures
    readInstructions(fp);

    // Close the file.
    fclose(fp); 

    // Begin to interpret
    execute();
    
    // debugging purposes...
    printTables();

    printf("\nProgram halted\n");
}

void readInstructions(FILE * fp)
{
   int address = 0;
   char opcode[OPCODE_SIZE];
   char operand[OPERAND_SIZE];
    
   printf("\n** Output **\n\n");

   while(fscanf(fp, "%s", opcode) != EOF){
       
       // operand check(read into operand var)
       if(hasOperand(opcode)){
           fscanf(fp, "%s", operand);
       } else{
           operand[0] = 0;
       }
       
       // insert the instruction into the instruction table
       insertInstruction(address, opcode, operand);
       address++;

       // discard any possible comments on current line
       discardline(fp);
   }
}

void execute()
{
    //printInstructionTable();
	
    int pc = 0;
    char opcode[OPCODE_SIZE];
    char operand[OPERAND_SIZE];
    
    while(strcmp(opcode, "halt")){
        
        fetchInstruction(pc, opcode, operand);
        
        // execution of current instruction
        if(strcmp(opcode, "nop") == 0){pc = nop(pc);}
        else if(strcmp(opcode, "add") == 0){pc = add(pc);}
        else if(strcmp(opcode, "sub") == 0){pc = sub(pc);}
        else if(strcmp(opcode, "mul") == 0){pc = mul(pc);}
        else if(strcmp(opcode, "divide") == 0){pc = divide(pc);}
        else if(strcmp(opcode, "get") == 0){pc = get(pc, operand);}
        else if(strcmp(opcode, "put") == 0){pc = put(pc, operand);}
        else if(strcmp(opcode, "push") == 0){pc = push(pc, operand);}
        else if(strcmp(opcode, "pop") == 0){pc = pop(pc, operand);}
        else if(strcmp(opcode, "not") == 0){pc = not(pc);}
        else if(strcmp(opcode, "and") == 0){pc = and(pc);}
        else if(strcmp(opcode, "or") == 0){pc = or(pc);}
        else if(strcmp(opcode, "testeq") == 0){pc = testeq(pc);}
        else if(strcmp(opcode, "testne") == 0){pc = testne(pc);}
        else if(strcmp(opcode, "testlt") == 0){pc = testlt(pc);}
        else if(strcmp(opcode, "testle") == 0){pc = testle(pc);}
        else if(strcmp(opcode, "testgt") == 0){pc = testgt(pc);}
        else if(strcmp(opcode, "testge") == 0){pc = testge(pc);}
        else if(strcmp(opcode, "jump") == 0){pc = jump(pc, operand);}
        else if(strcmp(opcode, "jf") == 0){pc = jf(pc, operand);}
    }
}


// discard rest of line (good for comments)
void discardline(FILE * fp)
{
    int newline;
    do
    {
        newline = fgetc(fp);
    } while ((char)newline != '\n' && (newline != EOF));
}
