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
	int matrix1[SIZE][SIZE], matrix2[SIZE][SIZE], result_matrix[SIZE][SIZE], i, j, offset = 0, accumulator;
	char *wbuffer, *rbuffer;
	DWORD written, read;
	HANDLE pipefd1[2], pipefd[2];
	PROCESS_INFORMATION piChild;
	STARTUPINFO siChild;
	SECURITY_ATTRIBUTES pipeSec = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE};

	//Get descriptors of first pipe
	pipefd[0] = GetStdHandle(STD_INPUT_HANDLE);
	pipefd[1] = GetStdHandle(STD_OUTPUT_HANDLE);

	//Allocate memory for buffers
	wbuffer = malloc(BUF_SIZE);
	rbuffer = malloc(BUF_SIZE);

	//Read matrix1 and matrix2 from first pipe
	ReadFile(pipefd[0], rbuffer, BUF_SIZE, &read, NULL);

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
	//Create the second pipe
	//Obtencion de información para la inicialización del proceso hijo
	GetStartupInfo(&siChild);
	//Creacion de la tubería sin nombre
	CreatePipe(&pipefd1[0], &pipefd1[1], &pipeSec, 0);

	//Write the matrix1 into wbuffer
	for(i = 0; i < SIZE; i++)
	{
		for(j = 0; j < SIZE; j++)
			sprintf(wbuffer + strlen(wbuffer), "%d ", matrix1[i][j]);
		sprintf(wbuffer + strlen(wbuffer), "\n");	
	}
	//Write the matrix2 into wbuffer
	for(i = 0; i < SIZE; i++)
	{
		for(j = 0; j < SIZE; j++)
			sprintf(wbuffer + strlen(wbuffer), "%d ", matrix2[i][j]);
		sprintf(wbuffer + strlen(wbuffer), "\n");	
	}
	//Write matrix1 and matrix2 to the second pipe
	WriteFile(pipefd1[1], wbuffer, strlen(wbuffer), &written, NULL);
	
	//Hereda el proceso hijo los manejadores de la tubería del proceso padre
	siChild.hStdInput = pipefd1[0];
	siChild.hStdError = GetStdHandle(STD_ERROR_HANDLE);
	siChild.hStdOutput = pipefd[1];
	siChild.dwFlags = STARTF_USESTDHANDLES;
	CreateProcess(NULL, "MatrixPipeWGrandChild", NULL, NULL, TRUE, 0, NULL, NULL, &siChild, &piChild);

	//Do multiplication
	for(int j = 0; j < SIZE; j++)
	{
		for(int k = 0; k < SIZE; k++)
		{
			accumulator = 0;
			for(int l = 0, m = 0; l < SIZE && m < SIZE; l++, m++)
				accumulator += matrix1[j][l] * matrix2[m][k];
			result_matrix[j][k] = accumulator;
		}
	}
	//Clean wbuffer
	for(i = 0; i < BUF_SIZE; i++)
		wbuffer[i] = '\0';
	//Write multiplication into wbuffer
	for(i = 0; i < SIZE; i++)
	{
		for(j = 0; j < SIZE; j++)
			sprintf(wbuffer + strlen(wbuffer), "%d ", result_matrix[i][j]);
		sprintf(wbuffer + strlen(wbuffer), "\n");	
	}
	//Write to first pipe the result of multiplication
	WriteFile(pipefd[1], wbuffer, strlen(wbuffer), &written, NULL);

	CloseHandle(piChild.hThread);
	CloseHandle(piChild.hProcess);
	
	return 0;
}
