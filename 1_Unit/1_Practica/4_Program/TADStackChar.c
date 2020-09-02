#include <stdlib.h>
#include "TADStackChar.h"

void InitializeC(StackChar *stack)
{
	*stack = NULL;
	return;
}

void PushC(StackChar *stack, elementc e)
{
	elementc *aux;

	aux = malloc(sizeof(elementc));
	*aux = e;
	aux->down = *stack;
	*stack = aux;

	return;
}

elementc PopC(StackChar *stack)
{
	elementc e, *aux;

	e = **stack;
	aux = *stack;
	*stack = (*stack)->down;
	free(aux);

	return e;	
}

bool EmptyC(StackChar *stack)
{
	if(*stack == NULL)
		return true;
	return false;	
}

elementc TopC(StackChar *stack)
{
    return **stack;
}

int ValueTopC(StackChar *stack)
{
	elementc *aux;
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

void DestroyC(StackChar *stack)
{
	elementc *aux;

	if(!EmptyC(stack))
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
