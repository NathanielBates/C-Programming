/*
Nathaniel Bates
COP 3402
4/15/2017
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

int symbolIndex(char *word);
int symbolTableDupCheck(char *dupCheck);
void errorDisplay(int error);
int variables();
int constants();
void statement();
void emit(int op, int r, int l, int m);
void expression();
void codeGenerator();
void term();
void factor();
void condition();
int rel_op();
void block();
int procedures();
void program();
void getNextToken();
void checkReservedWords(char *identifier);

typedef enum
{
    LIT = 1, RTN, LOD, STO, CAL, INC, JMP, JPC, SIO1, SIO2, SIO3,
    NEG, ADD, SUB, MUL, DIV, ODD, MOD, EQL, NEQ, LSS, LEQ, GTR, GEQ
} op_code;

char* reservedWords[] = { "const", "nul", "odd", "var", "procedure", "call", "begin", "end", "if", "then", "else", "while", "do", "read", "write" };

int error, codeIndex, symbolCounter, level, conditional, endCheck;
int currentToken, curReg, running = 1, linesOfCode, nestProc;
node *head = NULL;
node *walker = NULL;
char identError[20];
instruction genCode[MAX_CODE_LENGTH];
symbolTable symTable[Max_Symbol_Table_Size];

int main(int argc, char *argv[])
{
   char *temp = argv[2];
   int i = 0, length, curReg = -1, l = 0, a = 0, v = 0, level = -1;

   // Run the Lexer and generate Linked List of tokens
   head = lexical(argv[1]);
   // Run the code generator to produce assembly code
   program();

   if(temp != NULL)
      length = strlen(temp);

   // Generate flags for user input. Program will function differently based on
   // what values the user provides.
   for(i = 0; i < length; i++)
   {
      if(temp[i] == 'l')
         l = 1;
      if(temp[i] == 'a')
         a = 1;
      if(temp[i] == 'v')
         v = 1;
   }

   // Print out Lexeme list.
   if(l == 1)
      printLexemes(head);

   // If there were any errors, display them.
   if(error != 0)
      errorDisplay(error);
   else
   {
      // If we made it in here, the programm was syntactically valid
      printf("No errors, program syntactically correct.\n\n");
      if (a == 1)
      {
         // Prints out the assembly code of all the instructions that were generated
         printf("Assembly Code:\n");
         for(int k = 0; k < linesOfCode; k++)
            printf("%d %d %d %d\n", genCode[k].op, genCode[k].r, genCode[k].l, genCode[k].m);
         printf("\n");
      }
      // Runs the P-Machine which executes the instructions.
      virtualMachine(genCode, linesOfCode, v);
   }

   return 0;
}
//Program starts parsing through the program and calling the block
//descends in the structure of the program.
void program()
{
   currentToken = findToken(head->word);
   walker = head;
   level = -1;
   block();
   if(error != 0)
      return;

   if(currentToken != periodsym)
   {
      error = 10;
      return;
   }
   else
   {
      emit(SIO3, 0, 0, 3);

   }
}

// Block function checks the grammar for the block portions
// of the program.
void block()
{
   int space, vars = 0, procs = 0, consts = 0;
   int jmpAddress, procAddress;
   int i;

   level++;
   space = 4;
   jmpAddress = codeIndex;
   emit(JMP, 0, 0, 0);
   if(currentToken == constsym)
   {
      consts = constants();
      if(consts == 0)
         return;
   }

   if(currentToken == varsym)
   {
      vars = variables();
      if(vars == 0)
         return;
   }

   if(currentToken == constsym)
   {
      error = 16;
      return;
   }

   space += vars;

   if(currentToken == procsym && nestProc == 0)
      procs = procedures();

   genCode[jmpAddress].m = codeIndex;

   emit(INC, 0, 0, space);

   statement();


   if(level != 0)
      emit(RTN, 0, 0, 0);
   level--;
}

// Procedures function generates all information for each declared
// procedure.
int procedures()
{
   int value, symListIndex, procCount = 0;

   do{
      procCount++;
      getNextToken();
      if(currentToken != identsym)
      {
         error = 9;
         return 0;
      }
      value = symbolTableDupCheck(walker->word);
      if(value == 1)
      {
         error = 21;
         return 0;
      }

      checkReservedWords(walker->word);
      strcpy(symTable[symbolCounter].name, walker->word);
      symTable[symbolCounter].kind = 3;
      symTable[symbolCounter].level = level;
      symTable[symbolCounter++].addr = codeIndex;

      getNextToken();
      if(currentToken != semicolonsym)
      {
         error = 3;
         return 0;
      }
      getNextToken();
      block();
      getNextToken();


   } while(currentToken == procsym);


   return procCount;
}


int variables()
{
   int numOfVars = 0, token, nextToken, flag = 1,  value, commaFlag = 1;

   while(flag == 1)
   {
      token = findToken(walker->word);
      // Token is an identifier
      if(token == 29)
      {
         if(walker->next != NULL && (findToken(walker->next->word) != 2))
         {
            checkReservedWords(walker->next->word);
            error = 1;
            return 0;
         }

         getNextToken();
         while(flag == 1)
         {
            token = findToken(walker->word);

            if(token == 2)
            {
               // Makes sure no reserved words were used as variable or constant names
               checkReservedWords(walker->word);

               if(commaFlag == 0)
               {
                  error = 2;
                  return 0;
               }

               // Makes sure there are no duplicate uses of variable names.
               //value = symbolTableDupCheck(walker->word);
               //if(value == 1)
                  //return 0;

               numOfVars++;
               strcpy(symTable[symbolCounter].name, walker->word);

               // Fills symbol table values with type, lexi-level and address.
               symTable[symbolCounter].kind = 2;
               symTable[symbolCounter].level = level;
               symTable[symbolCounter++].addr = 3 + numOfVars;
               getNextToken();
               commaFlag = 0;
            }
            // Makes sure declarations are separated by commas
            else if(token == 17)
            {
               commaFlag = 1;
               getNextToken();
            }
            // Semicolons end variable declarations
            else if(token == 18)
               flag = 0;
            // If no semicolon is found, throw an error
            else
            {
               flag = 0;
               error = 3;
               return 0;
            }
         }
      }
      getNextToken();
   }

   return numOfVars;
}

int constants()
{
   int numOfConsts = 0, token, flag = 1,  value, commaFlag = 1;

   while(flag == 1)
   {
      token = findToken(walker->word);
      if(token == 28)
      {
         // If the next token is not an identifier, throw an error.
         if((walker->next != NULL) && (findToken(walker->next->word) != 2))
         {
            error = 4;
            return 0;
         }

         getNextToken();
         flag = 1;
         while(flag == 1)
         {
            token = findToken(walker->word);

            if(token == 2)
            {
               // Makes sure that constants are initialized using '='
               if((walker->next != NULL) && (findToken(walker->next->word) != 9))
               {
                  error = 5;
                  return 0;
               }
               // Makes sure constant names are not used multiple times.
               value = symbolTableDupCheck(walker->word);
               if(value == 1)
                  return 0;
               // Generates name for this token in the symbol table
               strcpy(symTable[symbolCounter].name, walker->word);
               symTable[symbolCounter].kind = 1;
               getNextToken();
            }
            else if(token == 3)
            {
               // Creates integer representation of the token and assigns it to the
               // value of the token in the symbol table
               value = atoi(walker->word);
               symTable[symbolCounter++].val = value;
               numOfConsts++;

               // If a comma or semicolon does not follow a constant initialization, throw an error.
               if(walker->next != NULL && findToken(walker->next->word) != 17 && findToken(walker->next->word) != 18)
               {
                  error = 6;
                  return 0;
               }

               getNextToken();
            }
            else if(token == 9)
            {
               // If a number does not follow '=', throw an error
               if((walker->next != NULL) && (findToken(walker->next->word) != 3))
               {
                  error = 7;
                  return 0;
               }

               getNextToken();
            }
            else if(token == 17)
            {
               // If an identifier does not follow a comma, throw an error.
               if((walker->next != NULL) && (findToken(walker->next->word) != 2))
               {
                  error = 8;
                  return 0;
               }
               commaFlag = 1;
               getNextToken();
            }
            // End constant declarations
            else if(token == 18)
               flag = 0;
            else
            {
               flag = 0;
               error = 3;
               return 0;
            }
         }
      }
      getNextToken();
   }
   return numOfConsts;
}

void statement()
{
   int index, lineNumTemp, lineNumTemp2;
   if(currentToken == identsym)
   {
      index = symbolIndex(walker->word);

      // Variable/constant is not in the symbol table.
      if(index == -1)
      {
         error = 19;
         strcpy(identError, walker->word);
         return;
      }
      // If the symbol is not an identifier, throw an error
      if(symTable[index].kind != 2)
      {
         error = 18;
         return;
      }

      getNextToken();

      // Identifier statements must be followed by :=
      if(currentToken != becomessym)
      {

         error = 17;
         return;
      }

      getNextToken();

      // generate expression.
      expression();

      if(error != 0)
         return;

      // If neither a semicolon or end finish this statement, throw an error
      if(currentToken != semicolonsym && currentToken != endsym)
      {
         error = 3;
         return;
      }

      emit(STO, curReg, level - symTable[index].level, symTable[index].addr);
      curReg--;
   }
   else if(currentToken == callsym)
   {
      getNextToken();
      if(currentToken != identsym)
      {
         error = 28;
         return;
      }

      index = findToken(walker->word);
      if(index == -1)
      {
         error = 19;
         strcpy(identError, walker->word);
         return;
      }

      index = symbolIndex(walker->word);
      if(symTable[index].kind != 3)
      {
         error = 29;
         return;
      }

      emit(CAL, 0, level - symTable[index].level, symTable[index].addr);
      getNextToken();

   }
   else if(currentToken == beginsym)
   {
      getNextToken();
      statement();
      if(error != 0)
         return;

      // Parses through statements until the very last statement
      while(currentToken == semicolonsym)
      {
         getNextToken();
         statement();
         if(error != 0)
            return;
      }
      // If a begin block does not end with an "end", throw an error
      if(currentToken != endsym)
      {
         error = 11;
         return;
      }
      getNextToken();
   }
   else if(currentToken == ifsym)
   {
      conditional = 1;
      condition();
      if(error != 0)
         return;
      conditional = 0;

      // the conditional must be followed by a "then"
      if(currentToken != thensym)
      {
         error = 22;
         return;
      }
      getNextToken();
      // Saves the current code line number of the program
      lineNumTemp = codeIndex;
      emit(JPC, curReg, 0, 0);
      curReg--;

      statement();
      if(findToken(walker->next->word) == elsesym)
         getNextToken();
      if(error != 0)
         return;
      if(currentToken == elsesym)
      {
         getNextToken();
         lineNumTemp2 = codeIndex;
         emit(JMP, 0, 0, 0);
         genCode[lineNumTemp].m = codeIndex;
         statement();
         genCode[lineNumTemp2].m = codeIndex;
      }
   else
      genCode[lineNumTemp].m = codeIndex;

   }
   else if(currentToken == whilesym)
   {
      lineNumTemp = codeIndex;
      conditional = 1;
      condition();
      if(error != 0)
         return;
      conditional = 0;
      lineNumTemp2 = codeIndex;
      emit(JPC, curReg, 0, 0);

      // "while" must be followed by "do"
      if(currentToken != dosym)
      {
         error = 24;
         return;
      }
      getNextToken();

      statement();
      if(error != 0)
         return;
      emit(JMP, 0, 0, lineNumTemp);
      // This is the address of this instruction
      genCode[lineNumTemp2].m = codeIndex;
   }
   else if(currentToken == readsym)
   {
      getNextToken();
      // "read" must be followed by an identifier (variable, to be specific)
      if(currentToken != identsym)
      {
         error = 25;
         return;
      }
      index = symbolIndex(walker->word);

      // Identifier was not found in the symbol table
      if(index == -1)
      {
         error = 19;
         strcpy(identError, walker->word);
         return;
      }
      // Must be an identifier; otherwise, throw an error
      if(symTable[index].kind != 2)
      {
         error = 18;
         return;
      }
      getNextToken();

      if(currentToken != semicolonsym && currentToken != endsym)
      {
         error = 3;
         return;
      }
      if(currentToken == endsym)
         endCheck = 1;
      curReg++;

      // Generate assembly code
      emit(SIO2, curReg, 0, 2);
      emit(STO, curReg, level - symTable[index].level, symTable[index].addr);
      curReg--;
   }
   else if(currentToken == writesym)
   {
      getNextToken();
      // "write" must be followed by an identifier
      if(currentToken != identsym)
      {
         error = 25;
         return;
      }
      index = symbolIndex(walker->word);

      // Identifier was not found in the symbol table
      if(index == -1)
      {
         error = 19;
         strcpy(identError, walker->word);
         return;
      }
      curReg++;
      if(symTable[index].kind == 2)
         emit(LOD, curReg, level - symTable[index].level, symTable[index].addr);
      else if(symTable[index].kind == 1)
         emit(LIT, curReg, 0, symTable[index].val);

      emit(SIO1, curReg, 0, 0);
      curReg--;

      getNextToken();
      if(currentToken != semicolonsym && currentToken != endsym)
      {
         error = 3;
         return;
      }
      if(currentToken == endsym)
         endCheck = 1;
   }
}

void condition()
{
   int opCode;
   getNextToken();
   if(currentToken == oddsym)
   {
      getNextToken();
      expression();
      if(error != 0)
         return;
      emit(ODD, curReg, 0, 0);
   }
   else
   {
      expression();
      if(error != 0)
         return;
      // Generates the relational operator being used in the conditional
      opCode = rel_op();

      // Was not an operator
      if(opCode == 0)
      {
         strcpy(identError, walker->word);
         error = 23;
         return;
      }
      getNextToken();

      expression();
      if(error != 0)
         return;

      emit(opCode, curReg - 1, curReg - 1, curReg);
      curReg--;
   }
}

// Finds out what operator symbol the current token is
int rel_op()
{
   switch (currentToken)
   {
      case eqsym:
         return EQL;
      case neqsym:
         return NEQ;
      case lessym:
         return LSS;
      case leqsym:
         return LEQ;
      case gtrsym:
         return GTR;
      case geqsym:
         return GEQ;
      default:
         return 0;
   }
}

void expression()
{
   int addop;

   // Negates the value of the number/identifier
   if (currentToken == minussym)
   {
      addop = currentToken;
      getNextToken();
      term();

      if(error != 0)
         return;
      emit(NEG, curReg, curReg, 0);
   }

   else if(currentToken == plussym)
   {
      getNextToken();
      term();
   }
   else
   {
      term();
      if(error != 0)
         return;
   }

   // Keep running as long as there are expressions to be created
   while (currentToken == plussym || currentToken == minussym)
   {
      addop = currentToken;

      getNextToken();
      term();
      if(error != 0)
         return;

      // Add the values
      if (addop == plussym)
      {
          emit(ADD, curReg-1, curReg-1, curReg);    // ADD = 13
          curReg--;
      }
      // Subtract the values
      if (addop == minussym)
      {
          emit(SUB, curReg-1, curReg-1, curReg );    // SUB = 14
          curReg--;
      }
   }
}
void term()
{
   int multOp;

   factor();
   if(error != 0)
      return;
   // keep looping if there are more divide or multiplication symbols
   while (currentToken == slashsym || currentToken == multsym)
   {
      multOp = currentToken;
      getNextToken();
      factor();
      if(error != 0)
         return;
      if (multOp == multsym)     // Multiplication
      {
           emit( MUL, curReg-1, curReg-1, curReg ); // MUL = 15
           curReg--;
      }
      if (multOp == slashsym)    // Division
      {
           emit( DIV, curReg-1, curReg-1, curReg ); // DIV = 16
           curReg--;
      }
   }
}

void factor()
{
   int index, i;
   int value;

   if (currentToken == identsym)
   {
      if(conditional == 0)
      {
         value = findToken(walker->next->word);
         // If a valid token is not presented for a factor, throw an error
         if(value != rparentsym && value != 4 && value != 5 && value != 6 && value != 7 && value != semicolonsym && value != endsym)
         {
            error = 12;
            return;
         }
      }
      else
      {
         value = findToken(walker->next->word);
         // If it is a conditional, checks for different symbols than above
         if(value != eqsym && value != thensym && value != neqsym && value != lessym && value != dosym && value != leqsym && value != gtrsym && value != geqsym)
         {
            strcpy(identError, walker->word);
            error = 23;
            return;
         }
      }

      index = symbolIndex(walker->word);

      curReg++;

      if (symTable[index].kind == 2)
         emit( LOD, curReg, level - symTable[index].level, symTable[index].addr );

      else if (symTable[index].kind == 1)
         emit(LIT, curReg, 0, symTable[index].val);
      // Factors work on variables and constants only.
      else
      {
          error = 19;
          strcpy(identError, walker->word);
          return;
      }
      getNextToken();
   }

   // Storing a numeric literal from code.
   else if (currentToken == numbersym)
   {
      value = atoi(walker->word);
      curReg++;
      emit(LIT, curReg, 0, value);
      getNextToken();
   }
   // "(" expression ")"
   else if (currentToken == lparentsym)
   {
      value = findToken(walker->next->word);
      if(value != 2 && value != 3 && value != lparentsym && value != minussym && value != plussym)
      {
         error = 13;
         return;
      }
      getNextToken();
      expression();

      if (currentToken != rparentsym)
      {
         if(error != 0)
            return;

         error = 14;
         return;
      }

      getNextToken();
   }
   else
   {
      error = 15;
      strcpy(identError, walker->word);
      return;
   }
}

// Prints out the error associated with each error code
void errorDisplay(int error)
{
   switch(error)
   {
      case 1:
         printf("Error 1: var, const, or procedure must be followed by an identifier.\n"); break;
      case 2:
         printf("Error 2: declarations must be separated by a comma.\n"); break;
      case 3:
         printf("Error 3: ';' not found.\n"); break;
      case 4:
         printf("Error 4: const must be followed by an identifier.\n"); break;
      case 5:
         printf("Error 5: declarations must be followed by an '=' sign.\n"); break;
      case 6:
         printf("Error 6: identifier declaration must be followed by ',' or ';'.\n"); break;
      case 7:
         printf("Error 7: '=' sign must be followed by numeric value.\n"); break;
      case 8:
         printf("Error 8: ',' must be followed by a new identifier.\n"); break;
      case 9:
         printf("Error 9: 'begin' statement for program block not found.\n"); break;
      case 10:
         printf("Error 10: '.' not found.\n"); break;
      case 11:
         printf("Error 11: no final end statement.\n"); break;
      case 12:
         printf("Error 12: operand must follow identifier.\n"); break;
      case 13:
         printf("Error 13: identifier must follow left parenthesis.\n"); break;
      case 14:
         printf("Error 14: no right closing parenthesis.\n"); break;
      case 15:
         printf("Error 15: expression cannot begin with this symbol '%s'.\n", identError); break;
      case 16:
         printf("Error 16: constants must be declared first.\n"); break;
      case 17:
         printf("Error 17: assignment symbol must follow variable.\n"); break;
      case 18:
         printf("Error 18: only variables can be assigned values.\n"); break;
      case 19:
         printf("Error 19: '%s' not declared.\n", identError); break;
      case 20:
         printf("Error 20: code length to long.\n"); break;
      case 21:
         printf("Error 21: '%s' declared multiple times.\n", identError); break;
      case 22:
         printf("Error 22: then statement expected.\n"); break;
      case 23:
         printf("Error 23: '%s' is not a conditional op code.\n", identError); break;
      case 24:
         printf("Error 24: do statement expected.\n"); break;
      case 25:
         printf("Error 25: read or write must be followed by identifier.\n"); break;
      case 26:
         printf("Error 26: symbol '%s' not supported.\n", identError); break;
      case 27:
         printf("Error 27: '%s' is a reserved word.\n", identError); break;
      case 28:
         printf("Error 28: identifier must follow call statement.\n"); break;
      case 29:
         printf("Error 29: must call a procedure, not variable or constant.\n"); break;
   }
}

// Generates the assembly instructions based on the parameters passed in
void emit(int op, int r, int l, int m)
{
   // Program too long
   if(codeIndex > MAX_CODE_LENGTH)
      error = 20;
   else
   {
      genCode[codeIndex].op = op; // First register
      genCode[codeIndex].r = r; // Second register
      genCode[codeIndex].l = l; // Lexical level
      genCode[codeIndex++].m = m; // Address
      linesOfCode++;
   }
}

// Checks to see if an identifier has already been inserted into the symbol table
// before inserting it again
int symbolTableDupCheck(char *dupCheck)
{
   int i;
   for(i = 0; i < symbolCounter; i++)
   {
      if((strcmp(symTable[i].name, dupCheck)) == 0)
      {
         if(symTable[i].level == level)
         {
            error = 21;
            strcpy(identError, dupCheck);
            return 1;
         }

      }
   }

  return 0;
}

// Finds the value of the next token
void getNextToken()
{
   if(walker->next->word != NULL)
      walker = walker->next;
   currentToken = findToken(walker->word);

}

// Finds the index of the symbol in the symbol table
int symbolIndex(char *word)
{
   for(int j = 0; j < symbolCounter; j++)
      if(strcmp(symTable[j].name, word) == 0)
            return j;

   return -1;
}

// Function to check if a particular word is a reserved/key word
void checkReservedWords(char *identifier)
{
	token_type symbol = 0;

	if (!strcmp(identifier, "const"))
		symbol = 1;
	else if (!strcmp(identifier, "odd"))
		symbol = 1;
	else if (!strcmp(identifier, "var"))
		symbol = 1;
	else if (!strcmp(identifier, "procedure"))
		symbol = 1;
	else if (!strcmp(identifier, "call"))
		symbol = 1;
	else if (!strcmp(identifier, "begin"))
		symbol = 1;
	else if (!strcmp(identifier, "end"))
		symbol = 1;
	else if (!strcmp(identifier, "if"))
		symbol = 1;
	else if (!strcmp(identifier, "then"))
		symbol = 1;
	else if (!strcmp(identifier, "else"))
		symbol = 1;
	else if (!strcmp(identifier, "while"))
		symbol = 1;
	else if (!strcmp(identifier, "do"))
		symbol = 1;
	else if (!strcmp(identifier, "read"))
		symbol = 1;
	else if (!strcmp(identifier, "write"))
		symbol = 1;
	else if (!strcmp(identifier, "nul"))
		symbol = 1;

  // Throws an error if the word is, indeed, a reserved/key word.
  if (symbol)
  {
    error = 27;
    strcpy(identError, identifier);
    errorDisplay(error);
    exit(1);
  }

	return;
}
