/*
Nathaniel Bates
COP 3402
4/15/2017
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "header.h"

char words[100000][Max_Id_Length];
// Creates a new node for the linked list of lexemes
node *createNode()
{
   node *newNode = malloc(sizeof(node));
   newNode->next = NULL;

   return newNode;
}

// Function takes in the first character of the new word being read.
// It then gets each following character stopping when hitting a space.
// The word, or lexeme is then stored in the linked list.
node *identifier(char x, FILE *input, node *tail)
{
   int nextChar, position = 1;
   char *word = calloc(Max_Id_Length + 1, sizeof(char));

   // If lexeme is larger than allowed, program exits.
   word[0] = x;
   nextChar = fgetc(input);
   while(isalpha(nextChar) || isdigit(nextChar))
   {
      if(position > Max_Id_Length)
      {
         printf("Error: Variable name is larger than %d.\n", Max_Id_Length);
         exit(1);
      }
      word[position] = nextChar;
      position++;
      nextChar = fgetc(input);
   }

   // Adding lexeme to the linked list.
   tail->word = malloc(strlen(word) + 1);
   strcpy(tail->word, word);
   tail->next = createNode();

   // Free word when no longer needed, the file pointer is moved back 1 byte.
   free(word);
   if(nextChar != EOF)
      fseek(input, -1, SEEK_CUR);

   // Lexeme is printed out as part of original source code.
   //printf("%s", tail->word);
   return tail->next;

}

// Function takes in the first number of the new lexeme being read.
// It then gets each following character stopping when hitting a space.
// The word, or lexeme is then stored in the linked list.
node *number(char x, FILE *input, node *tail)
{
   int nextChar, position = 1;
   char *word = calloc(Max_Number_Length + 1, sizeof(char));

   // If number is longer than allowed, program exits.
   word[0] = x;
   nextChar = fgetc(input);
   if(isalpha(nextChar))
   {
      printf("Error: Identifiers do not begin with numbers.\n");
      exit(1);
   }
   while(isdigit(nextChar))
   {
      if(position > Max_Number_Length)
      {
         printf("Error: Number length is greater than %d\n", Max_Number_Length);
         exit(1);
      }
      word[position] = nextChar;
      position++;
      nextChar = fgetc(input);
      if(isalpha(nextChar))
      {
         printf("Error: Identifiers do not begin with numbers.\n");
         exit(1);
      }
   }

   // Adding lexeme to linked list.
   tail->word = malloc(strlen(word) + 1);
   strcpy(tail->word, word);
   tail->next = createNode();

   // Free word and move file pointer back 1 byte.
   free(word);
   if(nextChar != EOF)
      fseek(input, -1, SEEK_CUR);

   // Lexeme is printed out as part of original source code.
   //printf("%s", tail->word);
   return tail->next;

}

node *symbol(char x, FILE *input, node *tail)
{
   char *symbol = calloc(3, sizeof(char));
   int position = 1;
   char y;
   symbol[0] = x;

   switch(x)
   {
      // Looks for the beginning and ending of comments. If end is found
      // then the end of the linked list is returned. Otherwise an Error
      // thrown for not finding the end of the comments.
      case '/':
      {
         y = fgetc(input);
         if(y == '*')
         {
            //printf("/%c", y);
            y = fgetc(input);
            while(1)
            {
               if(y == '*')
               {
                  //printf("%c", y);
                  y = fgetc(input);
                  if(y == '/')
                  {
                     //printf("%c", y);
                     return tail;
                  }
               }
               else
               {
                  //printf("%c", y);
                  y = fgetc(input);
                  if(y == EOF)
                  {
                     printf("Error: No closing for comments.\n");
                     exit(1);
                  }
               }
            }
         }
         else
            if(y != EOF)
               fseek(input, -1, SEEK_CUR);
         break;
      }
      // The assignment statement is looked for here. If not valid then
      // an invalid symbol error is thrown.
      case ':':
      {
         y = fgetc(input);
         if(y == '=')
            symbol[position] = y;
         else
         {
            symbol[position] = y;
            printf("Error: Invalid Symbol- : must be followed by =\n");
            exit(1);
         }
         break;
      }

      // The less than or equal symbol and the not equal symbol is looked
      // for here. If not found, then the symbol array just holds '<'.
      case '<':
      {
         y = fgetc(input);
         if(y == '=' || y == '>')
            symbol[position] = y;
         else
           if(y != EOF)
              fseek(input, -1, SEEK_CUR);
         break;
      }

      // The greater than or equal to symbol is looked for here. If not
      // found then the symbol array just holds '>'.
      case '>':
      {
         y = fgetc(input);
         if(y == '=')
            symbol[position] = y;
         else
           if(y != EOF)
               fseek(input, -1, SEEK_CUR);
         break;
      }

      //Nothing needs added to these symbols.
      case '+' :
      case '-' :
      case '*' :
      case '(' :
      case ')' :
      case '=' :
      case ',' :
      case '.' :
      case ';' :
         break;
   }

   // Symbol, or lexeme, is added to the linked list.
   tail->word = malloc(strlen(symbol) + 1);
   strcpy(tail->word, symbol);
   tail->next = createNode();

   // Lexeme is printed out as part of original source code.
   //printf("%s", tail->word);
   return tail->next;
}

// Here the token value is returned depending on the lexeme that
// is passed into the function.
int findToken(char *word)
{
   if(word == NULL)
      return 0;

   if(isalpha(word[0]))
   {
      if(strcmp(word, "const") == 0)
         return 28;
      else if(strcmp(word, "var") == 0)
         return 29;
      else if(strcmp(word, "procedure") == 0)
         return 30;
      else if(strcmp(word, "call") == 0)
         return 27;
      else if(strcmp(word, "begin") == 0)
         return 21;
      else if(strcmp(word, "end") == 0)
         return 22;
      else if(strcmp(word, "if") == 0)
         return 23;
      else if(strcmp(word, "then") == 0)
         return 24;
      else if(strcmp(word, "else") == 0)
         return 33;
      else if(strcmp(word, "while") == 0)
         return 25;
      else if(strcmp(word, "do") == 0)
         return 26;
      else if(strcmp(word, "read") == 0)
         return 32;
      else if(strcmp(word, "write") == 0)
         return 31;
      else if(strcmp(word, "odd") == 0)
         return 8;
      else
         return 2;
   }
   else if(isdigit(word[0]))
      return 3;

   else if(ispunct(word[0]))
   {
      int a = strlen(word);
      if(a < 2)
      {
         if(word[0] == '+')
            return 4;
         if(word[0] == '-')
            return 5;
         if(word[0] == '*')
            return 6;
         if(word[0] == '/')
            return 7;
         if(word[0] == '(')
            return 15;
         if(word[0] == ')')
            return 16;
         if(word[0] == '=')
            return 9;
         if(word[0] == ',')
            return 17;
         if(word[0] == '.')
            return 19;
         if(word[0] == '<')
            return 11;
         if(word[0] == '>')
            return 13;
         if(word[0] == ';')
            return 18;

      // Error thrown if invalid symbol is passed in.
         printf("Error: Invalid symbol %s\n", word);
         exit(1);
      }

      if(a == 2)
      {
         if(strcmp(word, ":=") == 0)
            return 20;
         if(strcmp(word, "<=") == 0)
            return 12;
         if(strcmp(word, ">=") == 0)
            return 14;
         if(strcmp(word, "<>") == 0)
            return 10;

         // Error thrown if invalid symbol is passed in.
         printf("Error: Invalid symbol %s\n", word);
         exit(1);
      }

      if(a > 2)
      {
         printf("Error: Invalid symbol %s\n", word);
         exit(1);
      }
   }
   else
      return 0;
}
char *getSymbol(int number)
{
   switch(number)
   {
      case 1: return "nulsym";
      case 2: return "identsym";
      case 3: return "numbersym";
      case 4: return "plussym";
      case 5: return "minussym";
      case 6: return "multsym";
      case 7: return "slashsym";
      case 8: return "oddsym";
      case 9: return "eqsym";
      case 10: return "neqsym";
      case 11: return "lessym";
      case 12: return "leqsym";
      case 13: return "gtrsym";
      case 14: return "geqsym";
      case 15: return "lparentsym";
      case 16: return "rparentsym";
      case 17: return "commasym";
      case 18: return "semicolonsym";
      case 19: return "periodsym";
      case 20: return "becomessym";
      case 21: return "beginsym";
      case 22: return "endsym";
      case 23: return "ifsym";
      case 24: return "thensym";
      case 25: return "whilesym";
      case 26: return "dosym";
      case 27: return "callsym";
      case 28: return "constsym";
      case 29: return "varsym";
      case 30: return "procsym";
      case 31: return "writesym";
      case 32: return "readsym";
      case 33: return "elsesym";
   }
}
void printLexemes(node *head)
{
   int varCounter = 0, token, i, counter = 0;
   char tokens[100000];
   char *symbol = NULL;

   node *walker = head;
   while(walker->next != NULL)
   {
      token = findToken(walker->word);
      if(token == 2 || token == 3)
         strcpy(words[varCounter++], walker->word);

      tokens[counter++] = token;
      walker = walker->next;
   }
   printf("Lexeme List:\n");
   varCounter = 0;
   for(i = 0; i < counter; i++)
   {
      printf("%d ", tokens[i]);
      if(tokens[i] == 2 || tokens[i] == 3)
         printf("%s ", words[varCounter++]);
   }
   printf("\n\n");
   varCounter = 0;
   for(i = 0; i < counter; i++)
   {
      symbol = getSymbol(tokens[i]);
      printf("%s ", symbol);
      if(tokens[i] == 2 || tokens[i] == 3)
        printf("%s ", words[varCounter++]);
   }
   printf("\n\n");
}

node *lexical(char *fileName)
{
   int x, token;

   node *head, *tail;
   head = tail = createNode();

   // File is read in.
   FILE *ifp = fopen(fileName, "r");
   if(ifp == NULL)
   {
      printf("File not found.\n");
      exit(1);
   }

   // First character of the program is passed in.
   x = fgetc(ifp);

   // Entire file is processed with each lexeme added to the link list
   // in the order in which it is read.
   while(x != EOF)
   {
      if(isalpha(x))
         tail = identifier(x, ifp, tail);
      else if(isdigit(x))
         tail = number(x, ifp, tail);
      else if(ispunct(x))
         tail = symbol(x, ifp, tail);
      else
         ;

      x = fgetc(ifp);
   }

   fclose(ifp);

   return head;
}
