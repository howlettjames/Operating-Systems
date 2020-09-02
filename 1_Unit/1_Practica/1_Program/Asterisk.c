#include <stdio.h>
#include <stdbool.h>

void printSpaces(short s);
void printAst(short a);

int main(void)
{
	short n, i, j, outter_spaces, inner_spaces, asterisks;

	printf("Enter a number (it must be odd): ");
	scanf("%d", &n);

	outter_spaces = n / 2;
	inner_spaces = 0;
	asterisks = 3;
	for(i = 0; i < n / 2; i++)
	{
		if(i == 0)
		{
			printSpaces(outter_spaces);
			printf("*");
			printSpaces(2 * outter_spaces);
			printf("*");
		}
		else if(i == 1)
		{
			outter_spaces--;
			inner_spaces++;

			printSpaces(outter_spaces);
			printf("*");
			printSpaces(inner_spaces);	
			printf("*");
			printSpaces(2 * outter_spaces);
			printAst(asterisks);
		}
		else
		{
			outter_spaces--;
			inner_spaces += 2;
			asterisks += 2;

			printSpaces(outter_spaces);
			printf("*");
			printSpaces(inner_spaces);	
			printf("*");
			printSpaces(2 * outter_spaces);
			printAst(asterisks);
		}

		printf("\n");
	}

	for(i = 0; i < 2 * n; i++)
		printf("*");

	printf("\n");
	
	for(i = 0; i < (n / 2) - 1; i++)
	{
		printSpaces(outter_spaces);
		printf("*");
		printSpaces(inner_spaces);
		printf("*");
		printSpaces(2 * outter_spaces);
		printAst(asterisks);

		outter_spaces++;
		inner_spaces -= 2;
		asterisks -= 2;

		printf("\n");	
	}

	printSpaces(outter_spaces);
	printf("*");
	printSpaces(2 * outter_spaces);
	printf("*");	

	return 0;
}

void printSpaces(short s)
{
	for(int i = 0; i < s; i++)
		printf(" ");
}

void printAst(short a)
{
	for(int i = 0; i < a; i++)
		printf("*");
}