#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define SIZE 10

typedef unsigned char BYTE;

int main(void)
{
	BYTE i, j, k, l, m;
	int **matrix1, **matrix2, result_matrix[SIZE][SIZE];
	int det = 0, accumulator = 0;
	FILE *write_fp;

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

	printf("MULTIPLICATION\n");
	write_fp = fopen("Multiplication.txt", "w");
	for(j = 0; j < SIZE; j++)
	{
		for(k = 0; k < SIZE; k++)
		{
			accumulator = 0;
			for(l = 0, m = 0; l < SIZE && m < SIZE; l++, m++)
				accumulator += matrix1[j][l] * matrix2[m][k];
			result_matrix[j][k] = accumulator;
		}
	}

	for(j = 0; j < SIZE; j++)
	{
		for(k = 0; k < SIZE; k++)
		{
			fprintf(write_fp, "%d ", result_matrix[j][k]);
		}
		fprintf(write_fp, "\n");
	}
	fclose(write_fp);
	
	return 0;
}
