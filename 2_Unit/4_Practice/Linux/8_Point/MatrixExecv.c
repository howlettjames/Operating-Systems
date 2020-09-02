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
	char *argv[2];
	argv[0] = malloc(FILENAME_MAX);

	for(i = 0; i < 6; i++)
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
				strcpy(argv[0], "/home/james/Documents/ESCOM_SEMESTRE_5/2CM9_SISTEMAS_OPERATIVOS/2_Unit/4_Practice/8_Point/SumExecv.out");
				argv[1] = NULL;
				printf("Soy el hijo %d ejecutando: %s\n", i + 1, argv[0]);
				execv(argv[0], argv);	
			}
			else if(i == 1)
			{
				strcpy(argv[0], "/home/james/Documents/ESCOM_SEMESTRE_5/2CM9_SISTEMAS_OPERATIVOS/2_Unit/4_Practice/8_Point/SubtractionExecv.out");
				argv[1] = NULL;
				printf("Soy el hijo %d ejecutando: %s\n", i + 1, argv[0]);
				execv(argv[0], argv);			
			}
			else if(i == 2)
			{
				strcpy(argv[0], "/home/james/Documents/ESCOM_SEMESTRE_5/2CM9_SISTEMAS_OPERATIVOS/2_Unit/4_Practice/8_Point/MultiplicationExecv.out");
				argv[1] = NULL;
				printf("Soy el hijo %d ejecutando: %s\n", i + 1, argv[0]);
				execv(argv[0], argv);		
			}
			else if(i == 3)
			{
				strcpy(argv[0], "/home/james/Documents/ESCOM_SEMESTRE_5/2CM9_SISTEMAS_OPERATIVOS/2_Unit/4_Practice/8_Point/TransposeExecv.out");
				argv[1] = NULL;
				printf("Soy el hijo %d ejecutando: %s\n", i + 1, argv[0]);
				execv(argv[0], argv);		
			}
			else if(i == 4)
			{
				strcpy(argv[0], "/home/james/Documents/ESCOM_SEMESTRE_5/2CM9_SISTEMAS_OPERATIVOS/2_Unit/4_Practice/8_Point/InverseExecv.out");
				argv[1] = NULL;
				printf("Soy el hijo %d ejecutando: %s\n", i + 1, argv[0]);
				execv(argv[0], argv);		
			}
			else if(i == 5)
			{
				strcpy(argv[0], "/home/james/Documents/ESCOM_SEMESTRE_5/2CM9_SISTEMAS_OPERATIVOS/2_Unit/4_Practice/8_Point/ReadExecv.out");
				argv[1] = NULL;
				printf("Soy el hijo %d ejecutando: %s\n", i + 1, argv[0]);
				execv(argv[0], argv);		
			}
			exit(EXIT_SUCCESS);
		}	
	}
	exit(EXIT_SUCCESS);
}