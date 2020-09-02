#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#define SIZE 10

typedef unsigned char BYTE;

int determinant(int **matrix, int size);

int main(void)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	char *argv[] = {"SumExecv", "SubtractionExecv", "MultiplicationExecv", "InverseExecv", "TransposeExecv", "ReadExecv"};
	DWORD son_pid = GetCurrentProcessId();
	BYTE i = 0, j = 0;
	clock_t begin, end;
	int **matrix1, **matrix2;
	FILE *write_fp;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	
	matrix1 = malloc(SIZE * sizeof(int *));
	for(i = 0; i < SIZE; i++)
		matrix1[i] = malloc(SIZE * sizeof(int));

	matrix2 = malloc(SIZE * sizeof(int *));
	for(i = 0; i < SIZE; i++)
		matrix2[i] = malloc(SIZE * sizeof(int));

	//Create the matrices
	srand((unsigned) time(NULL));
	for(i = 0; i < SIZE; i++)
		for(j = 0; j < SIZE; j++)
		{
			matrix1[i][j] = rand() % 10;
			matrix2[i][j] = rand() % 10;
		}

	//Print the original two matrices
	write_fp = fopen("Original.txt", "w");
	for(i = 0; i < SIZE; i++)
	{
		for(j = 0; j < SIZE; j++)
			fprintf(write_fp, "%d ", matrix1[i][j]);
		fprintf(write_fp, "\n");
	}
	fprintf(write_fp, "\n");
	for(i = 0; i < SIZE; i++)
	{
		for(j = 0; j < SIZE; j++)
			fprintf(write_fp, "%d ", matrix2[i][j]);
		fprintf(write_fp, "\n");
	}
	fclose(write_fp);	

	begin = clock();
	//Call 5 processes to run concurrently
	for(i = 0; i < 5; i++)
		if(!CreateProcess(NULL, argv[i], NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
		{
			printf("Error: failed to invoke CreateProcess(%d)\n", GetLastError());
			return 0;
		}

	for(i = 0; i < 5; i++)
		WaitForSingleObject(pi.hProcess, INFINITE);
	
	if(!CreateProcess(NULL, argv[5], NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
		{
			printf("Error: failed to invoke CreateProcess(%d)\n", GetLastError());
			return 0;
		}
	WaitForSingleObject(pi.hProcess, INFINITE);
	end = clock();
	
	printf("\nExecution time: %f seconds\n", (double) (end - begin) / CLOCKS_PER_SEC);

	//Terminación controlada del proceso e hilo asociado de ejecución
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}