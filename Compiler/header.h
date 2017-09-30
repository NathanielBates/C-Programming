#ifndef HEADER_H
#define HEADER_H

#define Max_Symbol_Table_Size 100
#define Max_Id_Length 11
#define Max_Number_Length 5
#define MAX_STACK_HEIGHT 2000
#define MAX_CODE_LENGTH 500
#define MAX_LEXI_LEVELS 3

// Struct type contains the relevant information for each instruction.
typedef struct instruction{
  int op;   //OP code
  int r;    //Register
  int l;    //Level
  int m;    //M
}instruction;

// Node for Linked list that stores the lexemes of the program.
// (variables, constants, etc.).  Used for looping through to create
// the Lexeme Table and the Lexeme List.
typedef struct node
{
   char *word;
   int value;
   struct node *next;
} node;

// Symbol Table structure
typedef struct symbolTable
{
   int kind;
   char name[11];
   int val;
   int level;
   int addr;
}symbolTable;


// Enumerate the token types
typedef enum {
    nulsym = 1, identsym, numbersym, plussym, minussym,
    multsym,  slashsym, oddsym, eqsym, neqsym, lessym, leqsym,
    gtrsym, geqsym, lparentsym, rparentsym, commasym, semicolonsym,
    periodsym, becomessym, beginsym, endsym, ifsym, thensym,
    whilesym, dosym, callsym, constsym, varsym, procsym, writesym,
    readsym, elsesym } token_type;

// Functional Prototypes for VM
void virtualMachine(instruction code[], int numOfIns, int print);

void printInstructions(instruction code[], int pc);

void printCurrent(instruction code, int i);

void printAfter(instruction code, int i);

int base(int l);

int execute(instruction ir);

// Functional Prototypes for Lexical Analyzer
node *identifier(char x, FILE *input, node *tail);

node *number(char x, FILE *input, node *tail);

node *symbol(char x, FILE *input, node *tail);

int findToken(char *word);

node *lexical(char *fileName);

void printLexemes(node *head);

#endif
