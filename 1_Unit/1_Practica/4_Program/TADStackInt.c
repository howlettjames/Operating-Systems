#include <stdlib.h>
#include "TADStackInt.h"

void InitializeI(StackInt *stack)
{
	*stack = NULL;
	return;
}

void PushI(StackInt *stack, elementi e)
{
	elementi *aux;

	aux = malloc(sizeof(elementi));
	*aux = e;
	aux->down = *stack;
	*stack = aux;

	return;
}

elementi PopI(StackInt *stack)
{
	elementi e, *aux;

	e = **stack;
	aux = *stack;
	*stack = (*stack)->down;
	free(aux);

	return e;	
}

bool EmptyI(StackInt *stack)
{
	if(*stack == NULL)
		return true;
	return false;	
}

elementi TopI(StackInt *stack)
{
    return **stack;
}

int ValueTopI(StackInt *stack)
{
	elementi *aux;
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

void DestroyI(StackInt *stack)
{
	elementi *aux;

	if(!EmptyI(stack))
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
