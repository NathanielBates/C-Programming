/*
Nathaniel Bates
Text Prediction 
8/1/2016
*/

#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TextPrediction.h"

//Function creates a new node in a trie. Values initialized, root is returned.
TrieNode *createTrieNode(void)
{
   int i;
   TrieNode *root = malloc(sizeof(TrieNode));
   if(root == NULL)
      return NULL;

   for(i = 0; i < 26; i++)
      root->children[i] = NULL;

   root->count = 0;
   root->subtrie = NULL;

   return root;
}

//Function takes in a root to a trie along with a string to search for.
//The last node in the string is then returned.
TrieNode *getNode(TrieNode *root, char *str)
{
   int length, i, y = 0;
   TrieNode *temp = NULL;
   char buffer[1024] = {'\0'};

   if(root == NULL || str == NULL)
      return NULL;

   //Since all letters in the trie are lower case, letters in the string are
   //copied over to buffer and are converted to lower case. Buffer is then
   //used to locate the last node of the initial string.
   length = strlen(str);
   for(i = 0; i < length; i++)
   {
      buffer[i] = str[i];
      buffer[i] = tolower(buffer[i]);
   }

   //If the initial string is of length 1, No extra work is needed and the
   //node is returned.
   if(length == 1)
   {
      y = buffer[0] - 'a';
      if(root->children[y] == NULL)
         return NULL;

      return root->children[y];
   }

   //Y holds the index value of the node being looked at.  If the node is null,
   //it hasn't been created so null is returned, otherwise temp is set to the
   //node at index y. Temp is used to traverse the rest of the trie.
   y = buffer[0] - 'a';
   if(root->children[y] == NULL)
      return NULL;
   temp = root->children[y];

   for(i = 1; i < length; i++)
   {
      y = buffer[i] - 'a';
      if(temp->children[y] == NULL)
         return NULL;
      temp = temp->children[y];
   }

   //If the count at the last node is greater than 0, then the word is in the
   //trie.  Otherwise the string is not in the trie and null is returned.
   if(temp->count > 0)
      return temp;
   else
      return NULL;
}

//Inserts a string into the trie, no value is returned.
void insertString(TrieNode *root, char *str)
{
   int length, y, i;
   TrieNode *temp = NULL;

   if(root == NULL || str == NULL)
      return;

   //If the string is of length 1, we create a single node to represent the
   //string.  Y holds the value of the index where the node will be created.
   //The count at that node is updated.
   length = strlen(str);
   if(length == 1)
   {
      y = str[0] - 'a';
      if(root->children[y] == NULL)
      {
         root->children[y] = createTrieNode();
         root->children[y]->count++;
      }
      else
         root->children[y]->count++;

   }
   //Y continues to hold the index of the node that is to be created, while
   //temp is the pointer used to traverse the trie. The node representing
   //a letter of the string is created along the traversal.
   else
   {
      y = str[0] - 'a';
      if(root->children[y] == NULL)
      {
         root->children[y] = createTrieNode();
         temp = root->children[y];
      }
      else
         temp = root->children[y];

      for(i = 1; i < length; i++)
      {
         y = str[i] - 'a';

         //When i reaches the length of the string, the last node representing
         //the last letter of the string is created if it doesn't already
         //exist. The count at the last node is then updated.
         if(i == (length - 1))
         {
            if(temp->children[y] == NULL)
            {
               temp->children[y] = createTrieNode();
               temp = temp->children[y];
               temp->count++;
            }
            else
            {
               temp = temp->children[y];
               temp->count++;
            }

         }
         else
         {
            if(temp->children[y] == NULL)
            {
               temp->children[y] = createTrieNode();
               temp = temp->children[y];
            }
            else
               temp = temp->children[y];
         }
      }
   }
}

//Helper function to insert a string into a subtrie of a word. If a subtrie
//doesn't exist, it is created and the insertString function is used to insert
//the string. All updating is done in the insertString function as well.
void insertSubstring(TrieNode *root, char *str)
{
   TrieNode *temp = NULL;

   if(str == NULL)
      return;

   if(root->subtrie == NULL)
   {
      root->subtrie = createTrieNode();
      insertString(root->subtrie, str);
   }
   else
      insertString(root->subtrie, str);
}

//Function takes a string and removes all punctuation characters. The text
//array is the same length as the str array used in the buildTrie function.
void stripPunctuators(char *str)
{
   int i = 0, j = 0, length;
   char text[31000] = {'\0'};

   length = strlen(str);

   //All of the alphabetic characters from the string are copied over to the
   //text array.  2 counter variables are need to maintain accuracy, and j
   //also keeps track of the length of the "cleaned up" string.
   while(i != length)
   {
      if(isalpha(str[i]) || str[i] == ' ')
         text[j++] = str[i++];
      else
         i++;
   }

   //memset function used to clear out the original string so the cleaned up
   //string can be copied back over and used in the main function.
   memset(&str[0], 0, sizeof(str));

   for(i = 0; i < j; i++)
      str[i] = text[i];
   str[j] = '\0';
}

//Function takes in a text file and inserts every word into a trie.
//The str array is a size that can accomodate for the maximums detailed in the
//assignment pdf plus some extra.  Max word length per line is 30, max number
//of characters per word is 1024. Other char arrays are set to maximum
//size of a word. If the file fails to open, then the program exits.
TrieNode *buildTrie(char *filename)
{
   int x = 0, i = 0, length;
   char str[31000] = {'\0'};
   char sub[1024] = {'\0'};
   char word[1024] = {'\0'};
   TrieNode *temp = NULL;

   //The root of the trie is created.  File is opened and fgets is used for
   //each line in the file.  The line from the file then has the '\n'
   //character removed and the stripPunctuators function cleans it up.
   TrieNode *root = createTrieNode();

   FILE *ifp = fopen(filename, "r");
   if(ifp == NULL)
   {
      printf("Error: %s not found.\n", filename);
      exit(1);
   }

	while(fgets(str, 31000, ifp))
   {
      strtok(str, "\n");
      stripPunctuators(str);
      length = strlen(str);

      //The string is looped through and each word is copied into the word
      //array.  When a space is encountered, the word is placed into the trie.
      for(i = 0; i < length; i++)
      {
         if(isalpha(str[i]))
         {
            word[x++] = tolower(str[i]);
            word[x] = '\0';
         }

      //After the word is placed in the trie, it is copied into another array.
      //That array holds onto it in order to later place it into the subtrie
      //of the following word. Arrays are reset for the next set of words.
         else if(str[i] == ' ')
         {
            insertString(root, word);
            if(sub[0] != '\0')
            {
               temp = getNode(root, sub);
               insertSubstring(temp, word);
               memset(&sub[0], 0, sizeof(sub));
               sub[0] = '\0';
            }
            strcpy(sub, word);
            memset(&word[0], 0, sizeof(word));
            x = 0;
         }
      }

      //When we reach the last word of the string, the same process is done
      //one last time.  The character arrays are then all reset for the next
      //retrieval of the next line of text by fgets().
      insertString(root, word);
      if(sub[0] != '\0')
      {
         temp = getNode(root, sub);
         insertSubstring(temp, word);
         memset(&sub[0], 0, sizeof(sub));
         sub[0] = '\0';
      }
      x = 0;
      memset(&word[0], 0, sizeof(word));
      memset(&str[0], 0, sizeof(str));
      memset(&sub[0], 0, sizeof(sub));
   }

   //When the while loop is finished parsing the file, the file is closed and
   //the root of the trie is returned to the main function.
   fclose(ifp);
   return root;
}

//Recursive function that completely destroys every node in the trie.
TrieNode *destroyTrie(TrieNode *root)
{
   int i, j;
   if(root == NULL)
      return NULL;

   //Recursive calls for each child node in the trie.  If the subtrie is not
   //Null, then the children of the subtries are called as well.
   for(i = 0; i < 26; i++)
      if(root->children[i] != NULL)
         destroyTrie(root->children[i]);

   if(root->subtrie != NULL)
      for(i = 0; i < 26; i++)
         if(root->subtrie->children[i] != NULL)
            destroyTrie(root->subtrie->children[i]);

   free(root->subtrie);
   free(root);
}

//Helps the mostFrequentWord function by parsing the children of the first
//node of each frequent word.  Allows us to see if there is another word
//being represented in a certain node of a subtrie. Function assumes that
//the node being passed into the function is not null.
int frequentHelper(TrieNode *letter)
{
   int i, j = 0;
   int count[100];

   //All indices in array are set to a value that will never be utilized in
   //the program as a node value. INT_MAX is basically being used as a flag.
   for(i = 0; i < 26; i++)
      count[i] = INT_MAX;

   for(i = 0; i < 26; i++)
      if(letter->children[i] != NULL)
         count[j++] = i;

   if(count[1] != INT_MAX)
      return count[1];
   else
      return INT_MAX;
}

//Function finds the most frequent word in a subtrie of another word.
void mostFrequentWord(TrieNode *root, char *str)
{
   int i, hits[26], k = 0, max = 0, j = 1, length, letter, count1, count2;
   TrieNode *temp = NULL, *walker = NULL;
   char word[1024] = {'\0'};
   char buffer[1024] = {'\0'};

   //Empty string set if the subtrie of a root is null.
   if(root->subtrie == NULL)
   {
      str[0] = '\0';
      return;
   }

   //Hits array used to keep track of the children nodes of the subtrie
   //that are not null.  All indices in array are set to the "flag" value.
   for(i = 0; i < 26; i++)
      hits[i] = INT_MAX;

   //Subtrie is set to temp, the non-null nodes are determined. Temp is reset
   //to null since it will be used later, and that status will be checked.
   temp = root->subtrie;
   for(i = 0; i < 26; i++)
      if(temp->children[i] != NULL)
         hits[k++] = i;

   temp = NULL;

   //K is reset to 0 and the while loop loops through the hits array.
   //For each node stored in the array, the string being represented by
   //the subsequent nodes is determined and placed into str pointer.
   k = 0;
   while(hits[k] != INT_MAX)
   {
      walker = root->subtrie->children[hits[k]];
      str[0] = hits[k] + 'a';

   //Letter variable is used to check and see if there are multiple words
   //in the children nodes of the subtrie.  If letter = INT_MAX, then the
   //child node in the subtrie only represents a single word.
      letter = frequentHelper(walker);

      for(i = 0; i < 26; i++)
      {
         if(walker->children[i] != NULL)
         {
            str[j++] = i + 'a';
            str[j] = '\0';
            walker = walker->children[i];
            i = -1;
         }
      }

   //Saftey measure so the count in the trie isn't altered anywhere else.
   count1 = walker->count;

   //If letter doesn't equal INT_MAX, then it contains the index of the second
   //letter. The buffer array stores the second word, looping through the
   //subsequent nodes. Temp is at the last node of the word after the loop.
      if(letter != INT_MAX)
      {
         buffer[0] = hits[k] + 'a';
         buffer[1] = letter + 'a';
         j = 2;
         temp = root->subtrie->children[hits[k]]->children[letter];

         for(i = 0; i < 26; i++)
         {
            if(temp->children[i] != NULL)
            {
               buffer[j++] = i + 'a';
               buffer[j] = '\0';
               temp = temp->children[i];
               i = -1;
            }
         }
         count2 = temp->count;
      }

      //If temp is not NULL then it is compared with the last node of the
      //string that walker was traversing. If the count at temp is greater,
      //then count1 (walker->count) is updated, buffer then copied to str.
      if(temp != NULL)
      {
         if(count2 > count1)
         {
            memset(&str[0], 0, sizeof(str));
            count1 = count2;
            strcpy(str, buffer);
         }
      }

      //Max is initialized to 0 and the count variable at the last node of the
      //string (walker) is compared to the max variable.  If the count is
      //greater than max, max is then set to that number.  Str is copied to
      //the temporary word buffer and str is reset to null.
      if(count1 > max)
      {
         max = walker->count;
         strcpy(word, str);
         memset(&str[0], 0, sizeof(str));
      }

      //If there are two strings of an equal occurence count, then the strings
      //that comes first alphabetically is copied in the word buffer.
      else if((count1 == max) && max != 0)
      {
         if(str[0] < word[0])
            strcpy(word, str);
      }

      //If needed, str is reset for the next run through the loop, k is
      //incremented, and j is reset to 1 since j begins at the second letter.
      memset(&str[0], 0, sizeof(str));
      j = 1;
      k++;
   }

   //The word last stored in the word buffer is the most occuring word, so
   //it is copied back over to the string pointer and null terminated.
   length = strlen(word);
   for(i = 0; i < length; i++)
      str[i] = word[i];

   str[i] = '\0';
}

//Print function courtesy of Dr. Szumlanski!
void printTrieHelper(TrieNode *root, char *buffer, int k)
{
	int i;

	if (root == NULL)
		return;

	if (root->count > 0)
		printf("%s (%d)\n", buffer, root->count);

	buffer[k + 1] = '\0';

	for (i = 0; i < 26; i++)
	{
		buffer[k] = 'a' + i;
		printTrieHelper(root->children[i], buffer, k + 1);
	}

	buffer[k] = '\0';
}

//Print function courtesy of Dr. Szumlanski!
void printTrie(TrieNode *root, int useSubtrieFormatting)
{
	char buffer[1026];

	if (useSubtrieFormatting)
	{
		strcpy(buffer, "- ");
		printTrieHelper(root, buffer, 2);
	}
	else
	{
		strcpy(buffer, "");
		printTrieHelper(root, buffer, 0);
	}
}

//The main function calls the buildTrie function creating the resulting trie
//from the corpus text file. The input text file is parsed in the main function.
int main(int argc, char **argv)
{
   int i, n = 0, j = 0, sum, length;
   char str[1030] = {'\0'}, buff[1024] = {'\0'}, num[100] = {'\0'};

   TrieNode *root = NULL;
   TrieNode *temp = NULL;

   //Error message if a file isn't entered before compiling.
   if(argc < 3)
   {
      fprintf(stderr, "Proper syntax: %s <filename> <filename>\n", argv[0]);
      return 1;
   }

   //The corpus text is passed to the function to build the trie.
   root = buildTrie(argv[1]);

   //The second file is opened in main.  If file fails to open, program exits.
   FILE *ifp = fopen(argv[2], "r");
   if(ifp == NULL)
   {
      printf("Error: %s not found.\n", argv[2]);
      exit(1);
   }

   //Fgets function scans in each line of the text file.  Strtok removes the
   //new line character at the end of the string.  If an ! is encountered,
   //The entire trie is printed and str is then reset to null for the next line.
   while(fgets(str, 1030, ifp))
   {
      strtok(str, "\n");
      length = strlen(str);

      if(str[0] == '!')
      {
         printTrie(root, 0);
         memset(&str[0], 0, sizeof(str));
      }

      //If @ symbol encountered, the string following is parsed into the buff
      //array.  When a space is hit, nothing is done. The number encountered
      //after the string is parsed into num array, then converted to an integer.
      else if(str[0] == '@')
      {
         for(i = 1; i < length; i++)
         {
            if(isalpha(str[i]))
            {
               buff[j++] = str[i];
               buff[j] = '\0';
            }
            else if(str[i] == ' ')
               ;
            else if(isdigit(str[i]))
            {
               num[n++] = str[i];
               num[n] = '\0';
            }
         }
         sum = atoi(num);
         printf("%s", buff);

         //The for loop goes through and prints out the most frequently
         //occuring word in the previous words subtrie. After the next word
         //is inserted into the buff array courtesy of the mostFrequentWord
         //function, it's node is returned, buff is reset.  The subtrie in the
         //node returned is used to find the next word to be printed.
         for(n = 0; n < sum; n++)
         {
            temp = getNode(root, buff);
            if(temp == NULL || temp->subtrie == NULL)
               break;
            memset(&buff[0], 0, sizeof(buff));
            mostFrequentWord(temp, buff);
            printf(" %s", buff);
         }

         printf("\n");
      }

      //If a single word occupies a line in the input text file, then it is
      //printed along with the words that are represented in it's subtrie.
      //If the string's subtrie is null, or if the string is not in the trie,
      //the user is notified.  The string is then reset, the incremental
      //variables used are then reset to 0.
      else if(isalpha(str[0]))
      {
         printf("%s\n", str);
         temp = getNode(root, str);
         if(temp == NULL)
            printf("(INVALID STRING)\n");
         else if(temp->subtrie == NULL)
            printf("(EMPTY)\n");
         else
            printTrie(temp->subtrie, 1);
      }
      memset(&str[0], 0, sizeof(str));
      n = 0;
      j = 0;
   }

   //The trie and all of the subsequent subtries are freed.  The file is also
   //closed and all memory leaks are taken care of before exiting.
   destroyTrie(root);
   fclose(ifp);
   return 0;
}
