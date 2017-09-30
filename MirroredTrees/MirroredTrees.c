/*
Nathaniel Bates
Mirrored Binary Trees
*/

#include "MirroredTrees.h"
#include <stdio.h>
#include <stdlib.h>

//New struct called stack used to store values for the pre-order
//and post-order traversals when determining if trees are "Kindred Spirits".
typedef struct stack{

	int *order;
	int size;
	int capacity;

}stack;

//Function counts the number of nodes in a tree. Thank You Dr. Szumlanski!!
int countNodes(node *root)
{
   if (root == NULL)
      return 0;

   return 1 + countNodes(root->left) + countNodes(root->right);
}

//Function creates a new stack used to store a traversal. A Tree's root is
//passed into the function, the nodes of the tree are counted and the capacity
//is set to that number. The new stack is returned so values can be "pushed" in.

stack *newStack(node *a)
{
	stack *s = malloc(sizeof(stack));
	if(s == NULL)
	   return NULL;

	s->size = 0;
	s->capacity = countNodes(a);
	s->order = malloc(sizeof(int) * s->capacity);
	if(s->order == NULL)
	   return NULL;

	return s;
}

//Frees all memory associated with the stack and returns NULL.
//Thanks Dr. Szumlanski!
stack *destroyStack(stack *s)
{
	if(s == NULL)
	   return NULL;

	free(s->order);
	free(s);

	return NULL;
}

//Function pushes the value of a tree's node onto the stack.
//Thanks Dr. Szumlanski!
void push(stack *p, int data)
{
	if(p == NULL || p->size == p->capacity)
	{
		printf("Stack is full.\n");
		return;
	}
	p->order[p->size++] = data;
}

//Function traverses the tree recursiveley and inserts the value at each node
//onto the stack. The result is the pre-order traversal of the tree.
//Thanks Dr. Szumlanski! I altered his print pre-order traversal function to
//push values onto a stack, instead of printing the values at each node.
stack *insertPreOrder(node *a, stack *p)
{
   if(a == NULL)
      return NULL;

   push(p, a->data);
   insertPreOrder(a->left, p);
   insertPreOrder(a->right, p);

   return p;
}

//Function traverses the tree recursiveley and pushes the value at each node
//onto the stack. The result is the post-order traversal of the tree.
//Thanks Dr. Szumlanski! I altered his print post-order traversal function to
//push values onto a stack, instead of printing the values at each node.
stack *insertPostOrder(node *b, stack *p)
{
   if(b == NULL)
      return NULL;

   insertPostOrder(b->left, p);
   insertPostOrder(b->right, p);
   push(p, b->data);

   return p;
}

//This function simply creates a new node with the data being passed in and
//returns a pointer to this node.  Thanks again to Dr. Szumlanski!
node *newNode(int data)
{
	node *n = malloc(sizeof(node));
	if(n == NULL)
	   return NULL;

	n->data = data;
	n->left = n->right = NULL;

	return n;
}

//Function takes in two tree roots and determines if they are mirror images of
//each other. The recursive function compares the data of the left
//side of one tree to the right side of the other tree and vice versa.
int isReflection(node *a, node *b)
{
	if(a == NULL && b == NULL)
	   return 1;

	if(a == NULL || b == NULL)
	   return 0;

	if(a->data != b->data)
	   return 0;

	isReflection(a->left, b->right);
	isReflection(a->right, b->left);
}

//Function takes in the root of a tree and recursively traverses the left and
//right sides. The new tree being created is a mirror image of the other.
//Therefore the right child of the original tree will be the left child of the
//new tree and vice versa. A pointer to the new tree is then returned.
node *makeReflection(node *root)
{
	node *reflect;
   if(root == NULL)
	   return NULL;

	reflect = newNode(root->data);
	reflect->left = makeReflection(root->right);
	reflect->right = makeReflection(root->left);

	return reflect;
}

//Function takes in two tree roots and determines if they are "Kindred Spirits."
int kindredSpirits(node *a, node *b)
{
   stack *s = NULL, *p = NULL;
	int i, kin;

	if(a == NULL && b == NULL)
	   return 1;

	if(a == NULL || b == NULL)
	   return 0;

	s = newStack(a);
	p = newStack(b);

	//If the number of nodes in each tree are not equal, then 0 is returned since
	//the pre-order traversal of one tree must be equal to the post-order
	//traversal of the other tree. Stacks are destroyed to prevent memory leaks.
	if(s->capacity != p->capacity)
	{
		destroyStack(s);
		destroyStack(p);
		return 0;
	}

	//Stacks containing the pre-order traversal of one tree and the post-order
	//traversal of the other tree are created.
	insertPreOrder(a, s);
	insertPostOrder(b, p);

	//If both trees are of size 1 and contain the same data, 1 is returned
	//since they are "Kindred Spirits", if not 0 returned, stacks are destroyed.
	if(s->capacity == 1 && p->capacity == 1)
	{
		if(s->order[0] == p->order[0])
		   kin = 1;
		else
		   kin = 0;

		destroyStack(s);
		destroyStack(p);

		return kin;
	}

	//The for loop compares the two stacks, if the values in the corresponding
	//indices are equal, then 1 is returned, otherwise, loop is broken out of
	//and 0 is returned.  The stacks are then destroyed to prevent memory leaks.
	for(i = 0; i < p->capacity; i++)
	{
		if(s->order[i] != p->order[i])
		{
			kin = 0;
			break;
		}
		else
		   kin = 1;
	}

	s = destroyStack(s);
	p = destroyStack(p);

	//The same process is done again below, except the traversal type being
	//evaluated on each tree is reversed. This way we can be sure that the
	//two trees being passed into the function are or are not "Kindred Spirits."
	//"The final value of 1 or 0 is determined and returned.
	if(kin == 0)
	{
		s = newStack(b);
		p = newStack(a);

		insertPostOrder(a, s);
		insertPreOrder(b, p);

		for(i = 0; i < p->capacity; i++)
		{
			if(s->order[i] != p->order[i])
			{
				kin = 0;
				break;
			}
			else
				kin = 1;
		}

		s = destroyStack(s);
		p = destroyStack(p);

	}

	return kin;
}

