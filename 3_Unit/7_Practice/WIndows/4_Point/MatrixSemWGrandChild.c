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
	int matrix1[SIZE][SIZE], matrix2[SIZE][SIZE], result_matrix[SIZE][SIZE], i, j, k;		
	int *buffer, *shm;
	char *shmid = "SharedMemory";
	HANDLE hMapFile, hSem2;
	
	//Open semaphore 2
	if((hSem2 = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "Semaphore2")) == NULL)
	{
		printf("Failed to invoke OpenSemaphore2: %d\n", GetLastError());
		return -1;
	}
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

	//Copy into the matrices the values stored into shared memory
	k = 0;
	for(i = 0; i < SIZE; i++)
		for(j = 0; j < SIZE; j++)
			matrix1[i][j] = buffer[k++];

	for(i = 0; i < SIZE; i++)
		for(j = 0; j < SIZE; j++)
			matrix2[i][j] = buffer[k++];

	//Do sum
	for(i = 0; i < SIZE; i++)
		for(j = 0; j < SIZE; j++)
			result_matrix[i][j] = matrix1[i][j] + matrix2[i][j];

	//Halt until Multiplication is done
	Sleep(1000);

	//Test control of semaphore 2
	WaitForSingleObject(hSem2, INFINITE);
	//printf("B\n");
	//Write sum into shared memory starting from -2000 marker at 3 * SIZE * SIZE
	k = 3 * SIZE * SIZE;
	for(i = 0; i < SIZE; i++)
		for(j = 0; j < SIZE; j++)
			buffer[k++] = result_matrix[i][j];			
	
	//Free semaphore 2
	if(!ReleaseSemaphore(hSem2, 1, NULL))
	{
		printf("Falla al invocar ReleaseSemaphore: %d\n", GetLastError());
	}
	UnmapViewOfFile(shm);
	CloseHandle(hMapFile);

	return 0;
}
