/*
Nathaniel Bates
COP 3402
4/15/2017
*/

#include <stdio.h>
#include <stdlib.h>
#include "header.h"

// Global variables and arrays used for the stack, registers, program counter,
// stack pointer, base pointer, and activation record information storage.
// regValue 2d array used to store SIO 0 0 3 info to print at the end.
int stack[MAX_STACK_HEIGHT], arNew[MAX_STACK_HEIGHT];
int regFile[16];
int regValue[MAX_CODE_LENGTH][MAX_CODE_LENGTH];
int halt, pc, sp, arCounter, bp = 1, regCounter, scanFlag, printFlag;

char* opCodes[] = {"", "lit", "rtn", "lod", "sto", "cal", "inc", "jmp", "jpc",
"sio", "sio", "sio", "neg", "add", "sub", "mul", "div", "odd", "mod", "eql",
"neq", "lss", "leq", "gtr", "geq"};

// Prints out the instructions for the input file using the strings
// that represent the op codes. 2 seperate printf statements to align
// for spacing when the instruction number is greater than 1 digit.
void printInstructions(instruction code[], int pc)
{
   int i;

   printf("\nLine OP     R   L   M\n\n");
   for(i = 0; i < pc; i++)
   {
      if(i >= 10)
      {
         printf("%d   %s    %d   %d   %d\n", i,
         opCodes[code[i].op], code[i].r, code[i].l, code[i].m);
      }
      else
      {
         printf("%d    %s    %d   %d   %d\n", i,
         opCodes[code[i].op], code[i].r, code[i].l, code[i].m);
      }
   }
}

// Prints the instruction code that precedes the stack.
void printCurrent(instruction code, int i)
{
   printf("%2d  %2s    %2d    %2d    %2d    ", i,
   opCodes[code.op], code.r, code.l, code.m);
}

// Prints the values of the program counter, base pointer, stack
// pointer, and the stack after the instruction has been executed.
void printAfter(instruction code, int i)
{
   printf("%2d    %2d    %2d   ", pc, bp, sp);

   for(int j = 1; j <= sp; j++)
   {
      for(int k = 0; k < MAX_CODE_LENGTH; k++)
         if(j == arNew[k])
            printf("| ");

      printf("%d ", stack[j]);
   }

// Prints the contents of the register as per instruction.
// If the last instruction is the final return, 0 is printed representing
// the stack.  When halt is 1 then the program stops running.
	if(code.op == 10)
	{
		//printf("\nPlease enter a value:	");
		//scanf("%d", &regFile[code.r]);
	}

   if(code.op == 2 && sp == 0)
   {
       printf("0");
       halt = 1;
   }

   printf("\n");
}

// Provided function that returns the base pointer of the specified
// lexicographical level.
int base(int l)
{
   int b1 = bp;

   while(l > 0)
   {
      b1 = stack[b1 + 1];
      l--;
   }
   return b1;
}

// Function executes the instruction based on the op code of the
// instruction passed to it.
int execute(instruction ir)
{
   switch(ir.op){

// Lit operation stores a constant into specified register.
   case 1:
      regFile[ir.r] = ir.m;
      pc++;
      break;

// Return operation returns from a function, pointers and counter updated.
   case 2:
      sp = bp - 1;
      bp = stack[sp + 3];
      pc = stack[sp + 4];
      break;

// Load operation loads a value from the stack into a register.
   case 3:
      regFile[ir.r] = stack[base(ir.l) + ir.m];
      pc++;
      break;

// Store operation stores a value from a register onto the stack.
   case 4:
      stack[base(ir.l) + ir.m] = regFile[ir.r];
      pc++;
      break;

// Call operation calls a function updates the values in the stack.
   case 5:
      pc++;
      stack[sp + 1] = 0;
      stack[sp + 2] = base(ir.l);
      stack[sp + 3] = bp;
      stack[sp + 4] = pc;
      bp = sp + 1;
      pc = ir.m;
      arNew[arCounter++] = sp + 1; // Used for printing bars.
      break;

// Increment operation moves the stack pointer to specified location.
   case 6:
      sp = sp + ir.m;
      pc++;
      break;

// Jump operation moves program counter to the specified instruction.
   case 7:
      pc = ir.m;
      break;

// Jump to specified instruction if specified register contains 0.
   case 8:
      if(regFile[ir.r] == 0)
         pc = ir.m;
      else
         pc++;
      break;

// SIO operation prints register contents, handled in print function.
// Values are stored for printing later.
   case 9:
      regValue[regCounter][0] = ir.r;
      regValue[regCounter++][1] = regFile[ir.r];
      printFlag = 1;
      pc++;
      break;

// SIO operation prompts user to enter a register number, handled in print function.
   case 10:
      scanFlag = 1;
      pc++;
      break;

// SIO operation halts the program.
   case 11:
      halt = 1;
      pc++;
      break;

// Negates a value in a register.
   case 12:
      regFile[ir.r] = -regFile[ir.l];
      pc++;
      break;

// Adds contents of 2 registers and stores result in another register.
   case 13:
      regFile[ir.r] = regFile[ir.l] + regFile[ir.m];
      pc++;
      break;

// Subtracts contents of 2 registers and stores result in another register.
   case 14:
      regFile[ir.r] = regFile[ir.l] - regFile[ir.m];
      pc++;
      break;

// Multiplies contents of 2 registers and stores result in another register.
   case 15:
      regFile[ir.r] = regFile[ir.l] * regFile[ir.m];
      pc++;
      break;

// Divides contents of 2 registers and stores result in another register.
   case 16:
      regFile[ir.r] = regFile[ir.l] / regFile[ir.m];
      pc++;
      break;

// Determines if content of a register is odd or not.
   case 17:
      regFile[ir.r] = regFile[ir.r] % 2;
      pc++;
      break;

// The modulus of the contents of 2 registers and stores result in a register.
   case 18:
      regFile[ir.r] = regFile[ir.l] % regFile[ir.m];
      pc++;
      break;

// Determines if the contents of 2 registers are equal, if so then 1 is stored.
   case 19:
      if(regFile[ir.l] == regFile[ir.m])
         regFile[ir.r] = 1;
      else
         regFile[ir.r] = 0;
      pc++;
      break;

// Determines if the contents of 2 registers are unequal, if so then 1 is stored.
   case 20:
      if(regFile[ir.l] != regFile[ir.m])
         regFile[ir.r] = 1;
      else
         regFile[ir.r] = 0;
      pc++;
      break;

// Less than operation on contents of registers, 1 or 0 is stored.
   case 21:
      if(regFile[ir.l] < regFile[ir.m])
         regFile[ir.r] = 1;
      else
         regFile[ir.r] = 0;
      pc++;
      break;

// Less than or equal to operation on register contents, 1 or 0 stored.
   case 22:
      if(regFile[ir.l] <= regFile[ir.m])
         regFile[ir.r] = 1;
      else
         regFile[ir.r] = 0;
      pc++;
      break;

// Greater than operation on register contents, 1 or 0 stored.
   case 23:
      if(regFile[ir.l] > regFile[ir.m])
         regFile[ir.r] = 1;
      else
         regFile[ir.r] = 0;
      pc++;
      break;

// Greater than or equal to operation on register contents, 1 or 0 stored.
   case 24:
      if(regFile[ir.l] >= regFile[ir.m])
         regFile[ir.r] = 1;
      else
         regFile[ir.r] = 0;
      pc++;
      break;
   }
   return 0;
}

// Main takes in a text file as a command line argument.
void virtualMachine(instruction code[], int numOfIns, int print)
{
   int i;
   instruction IR;

// If no instructions read in, then no program to run.
   if(numOfIns == 0)
   {
      printf("No program to run!\n");
      exit(1);
   }

// All of the instructions are printed out before the execution of
// each instruction is shown. Instructions are numbered starting at 0.
// IR is initialized to the first instruction, after the that then the
// program counter is used to determine the next instruction.
// Program stops running when all instructions have been executed
// and halt is set to 1 causing the loop to stop.
   if(print == 1)
   {
      printInstructions(code, numOfIns);
      printf("\nInitial Values               pc    bp    sp   Stack\n\n");
   }

   IR = code[0];
   while(!halt)
   {
      if(print == 1)
         printCurrent(IR, pc);
      execute(IR);
      if(print == 1)
         printAfter(IR, pc);
      if(scanFlag == 1)
      {
         printf("Enter a value: ");
         scanf("%d", &regFile[IR.r]);
         scanFlag = 0;
      }
      if(printFlag == 1)
      {
         printf("Value: %d\n", regFile[IR.r]);
         printFlag = 0;
      }
      IR = code[pc];
   }

// Printing out contents of registers from SI0 0 0 3 executions.
   if(regCounter > 0)
   {
      printf("\nRegister Values when Instruction SIO 0 0 3 executes:\n");
      for(int i = 0; i < regCounter; i++)
         printf("Register: %d    Value:  %d\n", regValue[i][0], regValue[i][1]);

      printf("\n");
   }

}
