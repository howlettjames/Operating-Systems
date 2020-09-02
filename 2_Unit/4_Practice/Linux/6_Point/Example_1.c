#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main()
{
	pid_t pid;
	char *argv[3];
	argv[0] = "/home/james/Documents/ESCOM_SEMESTRE_5/2CM9_SISTEMAS_OPERATIVOS/2_Unit/4_Practice/6_Point/hola.out";
	argv[1] = "Desde el hijo";
	argv[2] = NULL;
	if((pid = fork()) == 0)
	{
		printf("Soy el hijo ejecutando: %s\n", argv[0]);
		execv(argv[0], argv);
	}
	else
	{
		wait(0);
		printf("Soy el padre\n");
		exit(EXIT_SUCCESS);
	}
}