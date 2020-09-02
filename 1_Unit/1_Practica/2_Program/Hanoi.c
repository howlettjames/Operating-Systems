#include <stdio.h>

#define BEG 1
#define AUX 2
#define END 3

void hanoi(short n, short beg, short aux, short end);

int main(void)
{
	short n;

	printf("Enter a number: ");
	scanf("%d", &n);

	hanoi(n, BEG, AUX, END);

	return 0;
}

void hanoi(short n, short beg, short aux, short end)
{
	if(n == 1)
		printf("%d -> %d\n", beg, end);
	else
	{
		hanoi(n - 1, beg, end, aux);
		hanoi(1, beg, aux, end);
		hanoi(n - 1, aux, beg, end);
	}
}