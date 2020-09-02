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
	BYTE i, j, k;
	int **matrix1, **matrix2;
	int det = 0;
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

	printf("SUM\n");
	write_fp = fopen("Sum.txt", "w");
	for(j = 0; j < SIZE; j++)
		for(k = 0; k < SIZE; k++)
			matrix1[j][k] = matrix1[j][k] + matrix2[j][k];

	for(j = 0; j < SIZE; j++)
	{
		for(k = 0; k < SIZE; k++)
		{
			fprintf(write_fp, "%d ", matrix1[j][k]);
		}
		fprintf(write_fp, "\n");
	}
	fclose(write_fp);

	return 0;
}
