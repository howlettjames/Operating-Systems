#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 10

typedef unsigned char BYTE;

int main(void)
{
	BYTE i, j, k;
	int **matrix1, **matrix2;
	int det = 0;
	FILE *write_fp, *read_fp;


	matrix1 = malloc(SIZE * sizeof(int *));
	for(i = 0; i < SIZE; i++)
		matrix1[i] = malloc(SIZE * sizeof(int));

	matrix2 = malloc(SIZE * sizeof(int *));
	for(i = 0; i < SIZE; i++)
		matrix2[i] = malloc(SIZE * sizeof(int));

	read_fp = fopen("Original.txt", "rb");
	for(i = 0; i < SIZE; i++)
		for(j = 0; j < SIZE; j++)
			fscanf(read_fp, "%d", &matrix1[i][j]);

	for(i = 0; i < SIZE; i++)
		for(j = 0; j < SIZE; j++)
			fscanf(read_fp, "%d", &matrix2[i][j]);

	fclose(read_fp);
	
	printf("TRANSPOSE\n");
	write_fp = fopen("Transpose.txt", "w");
	for(j = 0; j < SIZE; j++)
		for(k = 0; k < SIZE; k++)
			matrix1[j][k] = matrix1[k][j];

	for(j = 0; j < SIZE; j++)
		for(k = 0; k < SIZE; k++)
			matrix2[j][k] = matrix2[k][j];	

	for(j = 0; j < SIZE; j++)
	{
		for(k = 0; k < SIZE; k++)
		{
			fprintf(write_fp, "%d ", matrix1[j][k]);
		}
		fprintf(write_fp, "\n");
	}

	fprintf(write_fp, "\n");
	for(j = 0; j < SIZE; j++)
	{
		for(k = 0; k < SIZE; k++)
		{
			fprintf(write_fp, "%d ", matrix2[j][k]);
		}
		fprintf(write_fp, "\n");
	}
	fclose(write_fp);
	
	return 0;
}
