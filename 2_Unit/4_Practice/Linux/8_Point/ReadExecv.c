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
	int **matrix1, **matrix2, result_matrix[SIZE][SIZE];
	FILE *read_fp;

	matrix1 = malloc(SIZE * sizeof(int *));
	for(i = 0; i < SIZE; i++)
		matrix1[i] = malloc(SIZE * sizeof(int));

	matrix2 = malloc(SIZE * sizeof(int *));
	for(i = 0; i < SIZE; i++)
		matrix2[i] = malloc(SIZE * sizeof(int));

	printf("Process 6\n");

	read_fp = fopen("Sum.txt", "rb");
	for(i = 0; i < SIZE; i++)
		for(j = 0; j < SIZE; j++)
			fscanf(read_fp, "%d", &result_matrix[i][j]);
	fclose(read_fp);

	printf("\t\t\t\tSUM\n");
	for(i = 0; i < SIZE; i++)
	{
		for(j = 0; j < SIZE; j++)
			printf("%5d ", result_matrix[i][j]);
		printf("\n");
	}

	read_fp = fopen("Subtraction.txt", "rb");
	for(i = 0; i < SIZE; i++)
		for(j = 0; j < SIZE; j++)
			fscanf(read_fp, "%d", &result_matrix[i][j]);
	fclose(read_fp);

	printf("\n\t\t\t    SUBTRACTION\n");
	for(i = 0; i < SIZE; i++)
	{
		for(j = 0; j < SIZE; j++)
			printf("%5d ", result_matrix[i][j]);
		printf("\n");
	}

	read_fp = fopen("Multiplication.txt", "rb");
	for(i = 0; i < SIZE; i++)
		for(j = 0; j < SIZE; j++)
			fscanf(read_fp, "%d", &result_matrix[i][j]);
	fclose(read_fp);

	printf("\n\t\t\t  MULTIPLICATION\n");
	for(i = 0; i < SIZE; i++)
	{
		for(j = 0; j < SIZE; j++)
			printf("%5d ", result_matrix[i][j]);
		printf("\n");
	}

	read_fp = fopen("Transpose.txt", "rb");
	for(i = 0; i < SIZE; i++)
		for(j = 0; j < SIZE; j++)
			fscanf(read_fp, "%d", &matrix1[i][j]);
	for(i = 0; i < SIZE; i++)
		for(j = 0; j < SIZE; j++)
			fscanf(read_fp, "%d", &matrix2[i][j]);
	fclose(read_fp);

	printf("\n\t\t\tTRANSPOSE OF MATRIX 1\n");
	for(i = 0; i < SIZE; i++)
	{
		for(j = 0; j < SIZE; j++)
			printf("%5d ", matrix1[i][j]);
		printf("\n");
	}
	printf("\n\t\t\tTRANSPOSE OF MATRIX 2\n");
	for(i = 0; i < SIZE; i++)
	{
		for(j = 0; j < SIZE; j++)
			printf("%5d ", matrix2[i][j]);
		printf("\n");
	}

	read_fp = fopen("Inverse.txt", "rb");
	for(i = 0; i < SIZE; i++)
		for(j = 0; j < SIZE; j++)
			fscanf(read_fp, "%d", &matrix1[i][j]);
	for(i = 0; i < SIZE; i++)
		for(j = 0; j < SIZE; j++)
			fscanf(read_fp, "%d", &matrix2[i][j]);
	fclose(read_fp);

	printf("\n\t\t\tINVERSE OF MATRIX 1\n");
	for(i = 0; i < SIZE; i++)
	{
		for(j = 0; j < SIZE; j++)
			printf("%14d ", matrix1[i][j]);
		printf("\n");
	}
	printf("\n\t\t\tINVERSE OF MATRIX 2\n");
	for(i = 0; i < SIZE; i++)
	{
		for(j = 0; j < SIZE; j++)
			printf("%14d ", matrix2[i][j]);
		printf("\n");
	}
	
	return 0;
}
