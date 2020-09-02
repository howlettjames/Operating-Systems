#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

typedef unsigned char BYTE;

int main()
{
	BYTE i;
	pid_t pid;
	char *argv[3];
	argv[0] = malloc(FILENAME_MAX);
	argv[1] = malloc(FILENAME_MAX);

	for(i = 0; i < 3; i++)
	{
		if((pid = fork()) != 0)
		{
			printf("Called process no.%d\n", pid);
			wait(0);
		}	
		else
		{
			if(i == 0)
			{
				strcpy(argv[0], "/home/james/Documents/ESCOM_SEMESTRE_5/2CM9_SISTEMAS_OPERATIVOS/1_Unit/1_Practica/3_Program/BalancedParentheses.out");
				argv[1] = NULL;
				printf("Soy el hijo %d ejecutando: %s\n", i + 1, argv[0]);
				execv(argv[0], argv);	
			}
			else if(i == 1)
			{
				strcpy(argv[0], "/home/james/Documents/ESCOM_SEMESTRE_5/2CM9_SISTEMAS_OPERATIVOS/1_Unit/2_Práctica/Linux/ChangeP.out");
				argv[1] = "/home/james/Documents/ESCOM_SEMESTRE_5/2CM9_SISTEMAS_OPERATIVOS/Tests";
				argv[2] = NULL;
				printf("Soy el hijo %d ejecutando: %s\n", i + 1, argv[0]);
				execv(argv[0], argv);			
			}
			else
			{
				strcpy(argv[0], "/home/james/Documents/ESCOM_SEMESTRE_5/2CM9_SISTEMAS_OPERATIVOS/2_Unit/4_Practice/7_Point/MatrixInverse.out");
				argv[1] = NULL;
				printf("Soy el hijo %d ejecutando: %s\n", i + 1, argv[0]);
				execv(argv[0], argv);		
			}
			exit(EXIT_SUCCESS);
		}	
	}
	exit(EXIT_SUCCESS);
}