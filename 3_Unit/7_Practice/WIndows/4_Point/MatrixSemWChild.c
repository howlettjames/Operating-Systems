//Program that uses process inter communication to process two matrices
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <windows.h>

#define SIZE 10
#define MEM_SIZE 5000 

int main(void)
{
	int matrix1[SIZE][SIZE], matrix2[SIZE][SIZE], result_matrix[SIZE][SIZE], i, j, k, accumulator;
	int *buffer, *shm;
	char *shmid = "SharedMemory";
	HANDLE hMapFile, hSem1, hSem2;
	PROCESS_INFORMATION piChild;
	STARTUPINFO siChild;

	//Open semaphore 1
	if((hSem1 = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "Semaphore1")) == NULL)
	{
		printf("Failed to invoke OpenSemaphore1: %d\n", GetLastError());
		return -1;
	}	
	//Open semaphore 2
	if((hSem2 = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "Semaphore2")) == NULL)
	{
		printf("Failed to invoke OpenSemaphore2: %d\n", GetLastError());
		return -1;
	}
	//Don't know what for but necessary to create child process correctly
	ZeroMemory(&siChild, sizeof(siChild));
	siChild.cb = sizeof(siChild);
	ZeroMemory(&piChild, sizeof(piChild));

	//Open file mapping
	if((hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, shmid)) == NULL)
	{
		printf("Failed to open file of mapping from shared memory: (%i)\n", GetLastError());
		exit(EXIT_FAILURE);
	}
	//Access/Attach to shared memory
	if((shm = (int *)MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, MEM_SIZE)) == NULL)
	{
		printf("Failed to access to shared memory: (%i)\n", GetLastError());
		CloseHandle(hMapFile);
		exit(EXIT_FAILURE); 
	}
	//Make a pointer to the shared memory
	buffer = shm;

	//Create child process
	if(!CreateProcess(NULL, "MatrixSemWGrandChild", NULL, NULL, FALSE, 0, NULL, NULL, &siChild, &piChild))
	{
		printf("Failed to create child process\n");
		exit(EXIT_FAILURE);
	}
	//Test control of semaphore 1
	WaitForSingleObject(hSem1, INFINITE);
	//Test control of semaphore 2
	WaitForSingleObject(hSem2, INFINITE);
	//printf("A\n");
	//CRITIC SECTION
	//Copy into the matrices the values stored into shared memory
	k = 0;
	for(i = 0; i < SIZE; i++)
		for(j = 0; j < SIZE; j++)
			matrix1[i][j] = buffer[k++];

	for(i = 0; i < SIZE; i++)
		for(j = 0; j < SIZE; j++)
			matrix2[i][j] = buffer[k++];

	//Do multiplication
	for(j = 0; j < SIZE; j++)
	{
		for(k = 0; k < SIZE; k++)
		{
			accumulator = 0;
			for(int l = 0, m = 0; l < SIZE && m < SIZE; l++, m++)
				accumulator += matrix1[j][l] * matrix2[m][k];
			result_matrix[j][k] = accumulator;
		}
	}
	//Write multiplication into shared memory starting from -1000 marker at 2 * SIZE * SIZE
	k = 2 * SIZE * SIZE;
	for(i = 0; i < SIZE; i++)
		for(j = 0; j < SIZE; j++)
			buffer[k++] = result_matrix[i][j];			

	//Free semaphore 2
	if(!ReleaseSemaphore(hSem2, 1, NULL))
	{
		printf("Failed to invoke ReleaseSemaphore2: %d\n", GetLastError());
	}
	//Wait for GrandChild to terminate, necessary to not leave semaphore 1 abandoned
	WaitForSingleObject(piChild.hProcess, INFINITE);

	//Free semaphore 1
	if(!ReleaseSemaphore(hSem1, 1, NULL))
	{
		printf("Failed to invoke ReleaseSemaphore1: %d\n", GetLastError());
	}
	CloseHandle(piChild.hThread);
	CloseHandle(piChild.hProcess);
	UnmapViewOfFile(shm);
	CloseHandle(hMapFile);

	return 0;
}
