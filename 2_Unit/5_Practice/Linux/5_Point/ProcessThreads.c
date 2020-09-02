#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>

void *thread(void *arg);
void *thread1(void *arg);
void *thread2(void *arg);

int main(void)
{
	pid_t pid;
	pthread_t id_thread;
	int *no;

	if((pid = fork()) == 0)
	{
		pid = getpid();
		printf("I'm the son my pid is: %d\n", pid);
		for(int i = 0; i < 15; i++)
		{
			*no = i + 1;
			pthread_create(&id_thread, NULL, thread, no);
			pthread_join(id_thread, NULL);
		}
		exit(EXIT_SUCCESS);
	}
	else
	{
		pid = getpid();
		printf("I'm the father my pid is: %d\n", pid);
		wait(0);
	}

	return 0;
}

void *thread(void *arg)
{
	int *no = arg;
	pthread_t id_thread = pthread_self();
	printf("\tI'm thread %d my pid is: %ld\n", *no, id_thread);

	for(int i = 0; i < 10; i++)
	{
		*no = i + 1;
		pthread_create(&id_thread, NULL, thread1, no);
		pthread_join(id_thread, NULL);	
	}
	return NULL;
}

void *thread1(void *arg)
{
	int *no = arg;
	pthread_t id_thread = pthread_self();
	printf("\t\tI'm thread %d my pid is: %ld\n", *no, id_thread);

	for(int i = 0; i < 5; i++)
	{
		*no = i + 1;
		pthread_create(&id_thread, NULL, thread2, no);
		pthread_join(id_thread, NULL);			
	}
	return NULL;
}

void *thread2(void *arg)
{
	int *no = arg;
	pthread_t id_thread = pthread_self();
	printf("\t\t\tI'm thread %d Practica 5\n", *no);

	return NULL;
}