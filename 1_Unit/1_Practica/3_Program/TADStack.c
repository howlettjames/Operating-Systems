#include <stdlib.h>
#include "TADStack.h"

void Initialize(Stack *stack)
{
	*stack = NULL;
	return;
}

void Push(Stack *stack, element e)
{
	element *aux;

	aux = malloc(sizeof(element));
	*aux = e;
	aux->down = *stack;
	*stack = aux;

	return;
}

element Pop(Stack *stack)
{
	element e, *aux;

	e = **stack;
	aux = *stack;
	*stack = (*stack)->down;
	free(aux);

	return e;	
}

bool Empty(Stack *stack)
{
	if(*stack == NULL)
		return true;
	return false;	
}

element Top(Stack *stack)
{
    return **stack;
}

int ValueTop(Stack *stack)
{
	element *aux;
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

void Destroy(Stack *stack)
{
	element *aux;

	if(!Empty(stack))
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
