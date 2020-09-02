#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define VALOR 1

int main(void)
{
	int desc_arch[2];
	char bufer[100];

	if(pipe(desc_arch) != 0)
	{
		fprintf(stderr, "Error: Failed to create the pipe.\n");
		exit(EXIT_FAILURE);
	}

	if(fork() == 0)
	{
		while(VALOR)
		{
			read(desc_arch[0], bufer, sizeof(bufer));
			printf("Se recibió: %s\n", bufer);
		}
	}
	
	while(VALOR)
	{
		fgets(bufer, 100, stdin);
		write(desc_arch[1], bufer, strlen(bufer) + 1);	
	}

	return 0;
}