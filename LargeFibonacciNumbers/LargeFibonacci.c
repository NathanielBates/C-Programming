/*
Nathaniel Bates
COP 3502C
Huge Fibonacci
*/

#include "LargeFibGen.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Function returns the sum of the values stored in each HugeInteger struct.
HugeInteger *hugeAdd(HugeInteger *p, HugeInteger *q)
{
	HugeInteger *s = NULL;
	int i, x = 0;

	if(p == NULL || q == NULL)
       return NULL;

	if(p->length == 0 || q->length == 0)
	   return NULL;

	//HugeInteger s is allocated to hold the sum of values passed into function.
	s = malloc(sizeof(HugeInteger));
	if(s == NULL)
	   return NULL;

	if(p->length <= q->length)
	{
		//Integer array in s allocated to the length of the largest value + 1.
		//The additional space is allocated in case a new 10's place is
		//needed when adding the HugeInteger arrays.
		s->digits = malloc(sizeof(int) * (q->length + 1));
		if(s->digits == NULL)
		{
			free(s);
			return NULL;
		}

		//All values in newly allocated array are set to zero.
		//Safety feature to eliminate any garbage values.
		for(i = 0; i <= q->length; i++)
		   s->digits[i] = 0;

		s->length = q->length;

		//First loop handles the addition of the values in shorter array, to the
		//larger array values.  X variable holds a 1 or 0 and is carried over
		//to the next 10's place. If a 1 isn't carried over, x is reset to 0.
		for(i = 0; i < p->length; i++)
		{
			if((p->digits[i] + q->digits[i] + x) > 9)
			{
				s->digits[i] = (p->digits[i] + q->digits[i] + x) % 10;
				x = 1;
				s->digits[i + 1] = 1;
			}

			else
			{
				s->digits[i] = (p->digits[i] + q->digits[i] + x);
				x = 0;
			}
		}

		//Second loop takes care of the remaining values in the larger array.
		//X carries the 1 over if necessary and resets if needed.  There are
		//no more values to add from p, so the appropriate values are set in s.
		for(i = p->length; i < q->length; i++)
		{
			if((q->digits[i] + x) > 9)
			{
				s->digits[i] = (q->digits[i] + x) % 10;
				x = 1;
			}

			else
			{
				s->digits[i] = (q->digits[i] + x);
				x = 0;
			}
		}

		//If x is still equal to 1 after each iteration, then the extra space
		//allocated is used and set to 1, since another 10's place is needed.
		if(x == 1)
	       s->digits[s->length++] = x;

		return s;
	}

	//This segment of code is the exact same process as above. Only difference
	//is since q is longer, length values need adusted in malloc and loops.
	if(p->length > q->length)
		{
			s->digits = malloc(sizeof(int) * (p->length + 1));
			if(s->digits == NULL)
			{
				free(s);
				return NULL;
			}

			for(i = 0; i <= p->length; i++)
			   s->digits[i] = 0;

			s->length = p->length;

			for(i = 0; i < q->length; i++)
			{
				if((p->digits[i] + q->digits[i] + x) > 9)
				{
					s->digits[i] = (p->digits[i] + q->digits[i] + x) % 10;
					x = 1;
					s->digits[i + 1] = 1;
				}

				else
				{
					s->digits[i] = (p->digits[i] + q->digits[i] + x);
					x = 0;
				}
			}

			for(i = q->length; i < p->length; i++)
			{
				if((p->digits[i] + x) > 9)
				{
					s->digits[i] = (p->digits[i] + x) % 10;
					x = 1;
				}

				else
				{
					s->digits[i] = (p->digits[i] + x);
					x = 0;
				}
			}

			if(x == 1)
			   s->digits[s->length++] = x;

		return s;
	}
}

//Funcion frees all memory allocated to a HugeInteger structure.
HugeInteger *hugeDestroyer(HugeInteger *p)
{
	if(p == NULL)
       return NULL;

	free(p->digits);
	free(p);

	return NULL;
}

//Function takes in a string of numbers and incorporates it into a HugeInteger.
HugeInteger *parseString(char *str)
{
	HugeInteger *s = NULL;
	int i, k;
	
	if(str == NULL)
       return NULL;

	s = malloc(sizeof(HugeInteger));
	if(s == NULL)
	   return NULL;

	//String length is need in order to determine length of the integer array.
	//Also is needed when memory is allocated to the new struct.

	k = strlen(str);

	//When "" is passed, it is treated as 0, string length is 1.
	//String length is 1 when an integer up to 9 is passed into function.
	if(k == 0 || k == 1)
	{
		s->digits = malloc(sizeof(int));
		if(s->digits == NULL)
		{
			free(s);
			return NULL;
		}

		s->length = 1;

		if(k == 0)
	       s->digits[0] = 0;

		if(k == 1)
	       s->digits[0] = str[0] - '0';

		return s;
	}

	s->digits = malloc(sizeof(int) * k);
	if(s->digits == NULL)
	{
		free(s);
		return NULL;
	}

	s->length = 0;

	//Since integers are stored in reverse order for addition purposes, we
	//begin with the final integer in the string, then store it in the first
	//element and proceed forward in s, and in reverse for the string. 48 is
	//subtracted from ascii value to get correct integer for the empty cell.
	for (i = k - 1; i >= 0; i--)
	   s->digits[s->length++] = str[i] - '0';

	return s;
}

//Function takes in an unsigned int and stores it in HugeInteger format.
HugeInteger *parseInt(unsigned int n)
{
	int i, k = 0;
	unsigned int num = n;
	HugeInteger *s = NULL;

	if(n < 0)
	   return NULL;

	s = malloc(sizeof(HugeInteger));
	if(s == NULL)
	   return NULL;

	//If 0 is entered, HugeInteger struct containing 0 is created.
	if(n == 0)
	{
		s->digits = malloc(sizeof(int));
		if(s->digits == NULL)
		{
			free(s);
			return NULL;
		}
		s->digits[0] = 0;
		s->length = 1;
		return s;
	}

	//While loop calculates the length of the integer. K is incremented from 0.
	//K is the length of the integer and used for memory allocation.
	while(num != 0)
	{
		num /= 10;
		k++;
	}

	s->digits = malloc(sizeof(int) * k);
	if(s->digits == NULL)
	{
		free(s);
		return NULL;
	}

	s->length = k;

	//For loop calculates the integer that will be stored in each element of
	//the integer array in the struct. The mod operator gets the last digit
	//in the integer.  n is decremented by that amount so that last value is 0.
	//n is divided by 10 to get the next "last" digit of the integer.
	for(i = 0; i < k; i++)
	{
		s->digits[i] = n % 10;
		n -= s->digits[i];
		n /= 10;
	}

   return s;
}

//Function takes in a HugeInteger struct and converts it to an unsigned integer
//Only returns a value if value in the struct is between 0 and data type's Max.
unsigned int *toUnsignedInt(HugeInteger *p)
{
	unsigned int *num = NULL;
	unsigned int y = 0;
	int i, x = 1;

	if(p == NULL)
	   return NULL;

	//Immediately returns NULL if value in struct is to large for the type.
	if(p->length > 10 || (p->length == 10 && p->digits[9] > 4))
	   return NULL;

	num = malloc(sizeof(unsigned int));
	if(num == NULL)
	   return NULL;

	//Value num is pointing to is set to 0. Incremented in the for loop.
	*num = 0;

	//Loop takes each integer in the array and multiplies it by it's
	//corresponding value of 10, 100, 1000, ..., increments until it reaches
	//the data type's maximum.  If it exceeds the maximum, NULL is returned.
	for(i = 0; i < p->length; i++)
	{
		y = (p->digits[i] * x);
		if(UINT_MAX - y >= *num)
		{
			*num += y;
			x *= 10;
		}
		else
		{
			free(num);
			return NULL;
		}
	}

	return num;
}

//Function calculates the nth Fibonacci Number!!
HugeInteger *fib(int n)
{
	HugeInteger *s = NULL;
	HugeInteger *p = NULL;
	HugeInteger *fibby = NULL;

	int i;

	if(n < 0)
	   return NULL;

	//If 0, 1 or 2 is passed, then value returned immediately.
	if(n == 0)
	   return parseInt(0);

	if(n == 1 || n == 2)
	   return parseInt(1);

	//p is set to the first Fibonacci number and s to the second.
	p = parseInt(0);
	s = parseInt(1);

	//Loop continually adds the preceding Fibonacci numbers until n is reached.
	//After each call to hugeAdd, p is destroyed to prevent memory leaks.
	//p and s are reset, the nth Fibonacci number is stored in fibby.
	for(i = 2; i <= n; i++)
	{
		fibby = hugeAdd(p, s);
		p = hugeDestroyer(p);
		if(i != n)
		{
			p = s;
			s = fibby;
		}
	}

	//s is destroyed to prevent a memory leak.
	hugeDestroyer(s);
	return fibby;
}

