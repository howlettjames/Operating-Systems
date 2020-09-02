#include <stdlib.h>
#include "TADStackTree.h"

void InitializeT(StackTree *stack)
{
	*stack = NULL;
	return;
}

void PushT(StackTree *stack, elementt e)
{
	elementt *aux;

	aux = malloc(sizeof(elementt));
	*aux = e;
	aux->down = *stack;
	*stack = aux;

	return;
}

elementt PopT(StackTree *stack)
{
	elementt e, *aux;

	e = **stack;
	aux = *stack;
	*stack = (*stack)->down;
	free(aux);

	return e;	
}

bool EmptyT(StackTree *stack)
{
	if(*stack == NULL)
		return true;
	return false;	
}

elementt TopT(StackTree *stack)
{
    return **stack;
}

int ValueTopT(StackTree *stack)
{
	elementt *aux;
	int stack_size = 0;

	aux = *stack;
	if(aux != NULL)
	{
		stack_size++;
		while(aux->down != NULL)
		{
			stack_size++;
			aux = aux->down;
		}
	}	
	return stack_size;
}

void DestroyT(StackTree *stack)
{
	elementt *aux;

	if(!EmptyT(stack))
	{
		while(*stack != NULL)
		{				
			aux = (*stack)->down;
			free(*stack);
			*stack = aux;
		}		
	}
	return;
}
