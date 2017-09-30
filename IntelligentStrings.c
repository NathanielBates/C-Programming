/*
Nathaniel Bates
NID:  na089815
COP 3502C
Smart Arrays Assignment
6/10/2016
*/

#include "SmartArray.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function creates a new Smart Array Struct. Returns pointer to new struct.
SmartArray *createSmartArray(int length)
{
	int i;		
	
	// Pointer to the new struct allocated and Capacity is determined.
	// New Array of pointers to strings created.
	
	SmartArray *NewArray = malloc(sizeof(SmartArray));
	if(NewArray == NULL)
	   return NULL;
	
	if(length > DEFAULT_INIT_LEN)
	   NewArray->capacity = length;
	else
	   NewArray->capacity = DEFAULT_INIT_LEN;
	
	NewArray->array = malloc(sizeof(char *) * NewArray->capacity);
	if(NewArray->array == NULL)
	   return NULL;
	
	// All pointers in the array initialized to NULL and size set to 0.
	
	for(i = 0; i < NewArray->capacity; i++)
	   NewArray->array[i] = NULL;
	
	NewArray->size = 0;
	printf("-> Created new SmartArray of size %d.\n", NewArray->capacity);
	
	return NewArray;
}

// Free all memory associated with a Smart Array struct.
SmartArray *destroySmartArray(SmartArray *smarty)
{
	int i;
	
	//All elements are freed in reverse order of allocation.
	//This is to ensure all memory is freed, and there are no leaks.
	
	if(smarty == NULL)
	   return NULL;
	else
	{	
		for(i = 0; i < smarty->capacity; i++)
		   free(smarty->array[i]);
		
		free(smarty->array);
		free(smarty);
		
		return NULL;
	}
}

// Smart Array structure is expanded to have capacity to hold more strings.
SmartArray *expandSmartArray(SmartArray *smarty, int length)
{	
	int i, j;
	char **NewArray = NULL;
	
	if(length <= 0)
	   return smarty;
	
	if(smarty == NULL)
	   return NULL;
	
	// If the length is less than current capacity, nothing is changed.
	if(length <= smarty->capacity)
	   return smarty;
	
	// New 2d array allocated to copy the existing string addresses into.
	NewArray = malloc(sizeof(char *) * (length));
	if(NewArray == NULL)
	   return NULL;
	
	// The memory address of the old string is copied into the new array.
	// The new indexes are set to null so memory can be allocated later.
	
	for (i = 0; i < smarty->size; i++)
	   NewArray[i] = smarty->array[i];
	
	for(j = smarty->size; j < length; j++)
	   NewArray[j] = NULL;
	
	// Pointer to old array freed, and smarty->array now points to new array.
	// Capacity updated, expansion was successful, and pointer is returned.
	
	free(smarty->array);
	smarty->array = NewArray;
	smarty->capacity = length;
	printf("-> Expanded SmartArray to size %d.\n", length);
	
	return smarty;
}

// Smart Array's capacity is decreased eliminating null members of array.
SmartArray *trimSmartArray(SmartArray *smarty)
{
	int i;
	char **NewArray = NULL;

	if(smarty == NULL)
	   return NULL;

	// Less than or equal sign used as a safety measure.
	
	if(smarty->capacity <= smarty->size)
	   return smarty;
   
	// New 2d array allocated to copy the existing string addresses into.
	NewArray = malloc(sizeof(char *) * smarty->size);
	if(NewArray == NULL)
	   return NULL;
   
	// The memory address of the old string is copied into the new array.
	
	for(i = 0; i < smarty->size; i++)
	   NewArray[i] = smarty->array[i];	
	
	// Pointer to old array freed, and smarty->array now points to new array.
	// Capacity updated, trimming was successful, and pointer is returned.
	
	free(smarty->array);
	smarty->array = NewArray;
	smarty->capacity = smarty->size;
	printf("-> Trimmed SmartArray to size %d.\n", smarty->capacity);

	return smarty;
}

// Function inserts a new string into the next unused cell.
char* put(SmartArray *smarty, char *str)
{
	int i;
	
	if(smarty == NULL || str == NULL)
	   return NULL;
	
	// If there is no space to add a string, then the array is expanded.
	// Greater than or equal sign is used as a safety measure.

	if(smarty->size >= smarty->capacity)
	   expandSmartArray(smarty, (smarty->capacity * 2) + 1);
	
	// str is inserted in the first empty cell.

	for(i = 0; i < smarty->capacity; i++)
	{ 
		if(smarty->array[i] == NULL)
		{
			smarty->array[i] = malloc(sizeof(char) * (strlen(str) + 1));
			if(smarty->array[i] == NULL)
			   return NULL;
			
			strcpy(smarty->array[i], str);
			smarty->size++;
			
			return smarty->array[i];		
		}
	}
}		

// Function returns pointer to the specified index.
// If user tries to access an index that is NULL, or
// out of bounds then NULL is returned.

char *get(SmartArray *smarty, int index)
{
	if(smarty == NULL)
	   return NULL;

	else if (index >= smarty->size || index < 0)
	   return NULL;
	
	else
	   return smarty->array[index];	
}

// Function replaces current string in the array with a new string.
char *set(SmartArray *smarty, int index, char *str)
{
	// NULL is returned if parameters are NULL, or index is out of bounds.
	
	if(str == NULL || smarty == NULL)
	   return NULL;
	   
	if(get(smarty, index) == NULL)
	   return NULL;
	
	// If index is in use, then the string in that index is freed and str
	// replaces the string. Memory is allocated for the new string.
	
	if(smarty->array[index] != NULL)
	{
		free(smarty->array[index]);
		
		smarty->array[index] = malloc(sizeof(char) * (strlen(str) + 1));
		if(smarty->array[index] == NULL)
		   return NULL;
		
		strcpy(smarty->array[index], str);
		
		return smarty->array[index];	
	}
}

// Function inserts a new string into the array.
char *insertElement(SmartArray *smarty, int index, char *str)
{
	int i, j, n;
	char *temp = NULL;
	
	// Null is returned for Null parameters and/or if index is invalid.
	// If all cells are used, then the Smart Array is expanded.
	// str is put into the next unused cell, pointer to the cell returned.
	
	if(smarty == NULL || str == NULL || index < 0)
	   return NULL;
	
	if(smarty->size >= smarty->capacity)
		smarty = expandSmartArray(smarty, smarty->capacity * 2 + 1);
	
	if(index >= smarty->size)
	{
		n = getSize(smarty);
		put(smarty, str);
		return smarty->array[n];
	}
	 
	// Space is allocated in temp for the string to be inserted.
		
	temp = malloc(sizeof(char) * (strlen(str) + 1));
	if(temp == NULL)
	   return NULL;
	
	strcpy(temp, str);
	
	// Since space is already allocated, we use simple pointer arithmetic to
	// point the next address in the array to the previous address. Since only
	// one string is inserted, we simply begin at the last index, which is 
	// smarty->size, and move towards the beginning of the array.
	
	for(j = smarty->size; j > index; j--)
	   smarty->array[j] = smarty->array[j - 1];
	
	// The index the string is inserted into is now pointed to its address.
		
	smarty->array[index] = temp;
	smarty->size++;
	
	return smarty->array[index];
}

// Function removes a string from array and moves remaining strings over.
int removeElement(SmartArray *smarty, int index)
{
	int i;
	
	if(smarty == NULL || get(smarty, index) == NULL)
	   return 0;
	
	if(smarty->array[index] == NULL)
	   return 0;
	
	// If index is the last string in the array, no shifting is necessary.
	// 1 is returned since last string was removed.
	
	if(index == (smarty->size - 1))
	{
		free(smarty->array[index]);
		smarty->array[index] = NULL;
		smarty->size--;
		return 1;
	}
	
	// String at specified index removed, remaining strings shifted left.
	// This process continues untill the last element in the array is reached.
	
	else	
	{
		free(smarty->array[index]);
		for(i = index; i < smarty->size; i++)   
		{	
			smarty->array[i] = smarty->array[i + 1];
		}
		
		smarty->size--;
		return 1;
	}	
}

// Function returns the current size of the array of strings.
int getSize(SmartArray *smarty)
{
	if(smarty == NULL)
	   return -1;
	else	
	   return smarty->size;
}

// Function prints all strings in the array.
void printSmartArray(SmartArray *smarty)
{
	int i;
	
	if(smarty == NULL || smarty->array[0] == NULL)
	   printf("(empty array)\n");	
	else
	   for(i = 0; i < smarty->size; i++)
		  printf("%s\n", smarty->array[i]);	
}

