#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

typedef unsigned char BYTE;

int main(void)
{
	pid_t pid_son;
	BYTE i = 0, j = 0, k = 0, l = 0;
	int wstatus;

	printf("|+|I'm the main process.\n");
	for(i = 0; i < 10; i++)
	{
		pid_son = fork();
		if(pid_son != 0)
		{
			printf("|+|The pid of my son %d is %d\n", i + 1, pid_son);
			waitpid(pid_son, &wstatus, 0);
		}
		else
		{
		printf("\t|+|+|I'm son number: %d\n", i + 1);
		if(i == 0 || i == 9)
		{
		for(j = 0; j < 5; j++)
		{
		if((pid_son = fork()) == 0)
		{
		if(j == 4)
		{
			printf("\t\t\t|+|+|I'm son number: %d\n", j + 1);
			for(k = 0; k < 5; k++)
			{
				if((pid_son = fork()) == 0)
				{
					pid_son = getppid();
					printf("\t\t\t\t       The pid of my father is %d\n", pid_son);
					exit(EXIT_SUCCESS);	
				}
				else
				{
					printf("\t\t\t\t|+|+|+|The pid of my son %d is %d\n", k + 1, pid_son);	
					wait(&wstatus);
				}
			}
		}
		}
		else
		{
			printf("\t\t|+|+|The pid of the %d son is %d\n", j + 1, pid_son);
			break;
		}
		}
		waitpid(pid_son, &wstatus, 0);
		exit(EXIT_SUCCESS);
		}
		else if(i == 1 || i == 8)
		{
		for(j = 0; j < 4; j++)
		{
		if((pid_son = fork()) == 0)
		{
		if(j == 3)
		{
			printf("\t\t\t|+|+|I'm son number: %d\n", j + 1);
			for(k = 0; k < 4; k++)
			{
				if((pid_son = fork()) == 0)
				{
					pid_son = getppid();
					printf("\t\t\t\t       The pid of my father is %d\n", pid_son);
					exit(EXIT_SUCCESS);	
				}
				else
				{
					printf("\t\t\t\t|+|+|+|The pid of my son %d is %d\n", k + 1, pid_son);	
					wait(&wstatus);
				}
			}
		}
		}
		else
		{
			printf("\t\t|+|+|The pid of the %d son is %d\n", j + 1, pid_son);
			break;
		}
		}
		waitpid(pid_son, &wstatus, 0);
		exit(EXIT_SUCCESS);
		}
		else if(i == 2 || i == 7)
		{
		for(j = 0; j < 3; j++)
		{
		if((pid_son = fork()) == 0)
		{
		if(j == 2)
		{
			printf("\t\t\t|+|+|I'm son number: %d\n", j + 1);
			for(k = 0; k < 3; k++)
			{
				if((pid_son = fork()) == 0)
				{
					pid_son = getppid();
					printf("\t\t\t\t       The pid of my father is %d\n", pid_son);
					exit(EXIT_SUCCESS);	
				}
				else
				{
					printf("\t\t\t\t|+|+|+|The pid of my son %d is %d\n", k + 1, pid_son);	
					wait(&wstatus);
				}
			}
		}
		}
		else
		{
			printf("\t\t|+|+|The pid of the %d son is %d\n", j + 1, pid_son);
			break;
		}
		}
		waitpid(pid_son, &wstatus, 0);
		exit(EXIT_SUCCESS);
		}
		else if(i == 3 || i == 6)
		{
		for(j = 0; j < 2; j++)
		{
		if((pid_son = fork()) == 0)
		{
		if(j == 1)
		{
			printf("\t\t\t|+|+|I'm son number: %d\n", j + 1);
			for(k = 0; k < 2; k++)
			{
				if((pid_son = fork()) == 0)
				{
					pid_son = getppid();
					printf("\t\t\t\t       The pid of my father is %d\n", pid_son);
					exit(EXIT_SUCCESS);	
				}
				else
				{
					printf("\t\t\t\t|+|+|+|The pid of my son %d is %d\n", k + 1, pid_son);	
					wait(&wstatus);
				}
			}
		}
		}
		else
		{
			printf("\t\t|+|+|The pid of the %d son is %d\n", j + 1, pid_son);
			break;
		}
		}
		waitpid(pid_son, &wstatus, 0);
		exit(EXIT_SUCCESS);
		}
		else if(i == 4 || i == 5)
		{
		for(j = 0; j < 1; j++)
		{
		if((pid_son = fork()) == 0)
		{
		if(j == 0)
		{
			printf("\t\t\t|+|+|I'm son number: %d\n", j + 1);
			for(k = 0; k < 1; k++)
			{
				if((pid_son = fork()) == 0)
				{
					pid_son = getppid();
					printf("\t\t\t\t       The pid of my father is %d\n", pid_son);
					exit(EXIT_SUCCESS);	
				}
				else
				{
					printf("\t\t\t\t|+|+|+|The pid of my son %d is %d\n", k + 1, pid_son);	
					wait(&wstatus);
				}
			}
		}
		}
		else
		{
			printf("\t\t|+|+|The pid of the %d son is %d\n", j + 1, pid_son);
			break;
		}
		}
		waitpid(pid_son, &wstatus, 0);
		exit(EXIT_SUCCESS);
		}
		}	
	}

	return 0;
}