#include <stdio.h>
#include <stdlib.h>
#include "TADStack.h"

int main(void)
{
	char expression[100];
	short i;
	Stack stack;
	element e;

	Initialize(&stack);

	printf("Enter an expression: ");
	scanf("%s", expression);

	i = 0;
	while(expression[i] != '\0')
	{
		if(expression[i] == '(' || expression[i] == '[' || expression[i] == '{')
			Push(&stack, (element) {.value = '('});
		else if(expression[i] == ')' || expression[i] == ']' || expression[i] == '}')
		{
			if(!Empty(&stack))
				Pop(&stack);
			else
			{
				printf("NOT BALANCED\n");
				exit(EXIT_FAILURE);
			}
		}

		i++;
	}

	if(Empty(&stack))
		printf("BALANCED\n");
	else
		printf("NOT BALANCED\n");

	return 0;
}