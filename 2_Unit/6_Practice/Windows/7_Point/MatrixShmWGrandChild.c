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
	HANDLE hMapFile;

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

	//Wait until multiplication is written into shared memory	
	k = 3 * SIZE * SIZE;
	while(buffer[k] != -2000)
		Sleep(1000);

	//Write sum into shared memory starting from -2000 marker at 3 * SIZE * SIZE
	k = 3 * SIZE * SIZE;
	for(i = 0; i < SIZE; i++)
		for(j = 0; j < SIZE; j++)
			buffer[k++] = result_matrix[i][j];			
	//Put a marker to indicate end of sum
	buffer[k] = -3000;	

	UnmapViewOfFile(shm);
	CloseHandle(hMapFile);

	return 0;
}
