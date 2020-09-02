//Program that uses process inter communication to process two matrices
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <windows.h>

#define SIZE 10
#define BUF_SIZE 1000 

int main(void)
{
	int matrix1[SIZE][SIZE], matrix2[SIZE][SIZE], result_matrix[SIZE][SIZE], i, j, offset = 0;		
	char *wbuffer, *rbuffer;
	DWORD written, read;
	//To be congurent I declared both arrays but they can be ommited since we only use pipefd1[0] and pipefd[1]
	HANDLE pipefd[2], pipefd1[2];

	//Get descriptors
	pipefd1[0] = GetStdHandle(STD_INPUT_HANDLE);
	pipefd[1] = GetStdHandle(STD_OUTPUT_HANDLE);

	//Allocate memory for buffers
	wbuffer = malloc(BUF_SIZE);
	rbuffer = malloc(BUF_SIZE);

	//Read matrix1 and matrix2 from pipe
	ReadFile(pipefd1[0], rbuffer, BUF_SIZE, &read, NULL);

	//Read into matrix1 from rbuffer
	for(i = 0; i < SIZE; i++)
		for(j = 0; j < SIZE; j++)
		{
			sscanf(rbuffer, " %d%n", &matrix1[i][j], &offset);
			rbuffer += offset;
		}
	//Read into matrix2 from rbuffer	
	for(i = 0; i < SIZE; i++)
		for(j = 0; j < SIZE; j++)
		{
			sscanf(rbuffer, " %d%n", &matrix2[i][j], &offset);
			rbuffer += offset;
		}
	//Do sum
	for(i = 0; i < SIZE; i++)
		for(j = 0; j < SIZE; j++)
			result_matrix[i][j] = matrix1[i][j] + matrix2[i][j];

	//Write the resultx of sum into wbuffer
	for(i = 0; i < SIZE; i++)
	{
		for(j = 0; j < SIZE; j++)
			sprintf(wbuffer + strlen(wbuffer), "%d ", result_matrix[i][j]);
		sprintf(wbuffer + strlen(wbuffer), "\n");	
	}
	//Write to first pipe the result of sum
	WriteFile(pipefd[1], wbuffer, strlen(wbuffer), &written, NULL);
	
	return 0;
}
