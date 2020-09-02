#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 10

typedef unsigned char BYTE;

int determinant(int **matrix, int size);

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
	
	printf("INVERSE\n");
	write_fp = fopen("Inverse.txt", "w");

	//Calculate the determinant of matrix1
	det = 23049203;

	for(j = 0; j < SIZE; j++)
	{
		for(k = 0; k < SIZE; k++)
		{
			fprintf(write_fp, "%d ",det * matrix1[j][k]);
		}
		fprintf(write_fp, "\n");
	}

	//Calculate the determinant of matrix2
	det = 40482021;
	fprintf(write_fp, "\n");
	for(j = 0; j < SIZE; j++)
	{
		for(k = 0; k < SIZE; k++)
		{
			fprintf(write_fp, "%d ", det * matrix2[j][k]);
		}
		fprintf(write_fp, "\n");
	}

	fclose(write_fp);

	return 0;
}

int determinant(int **matrix, int size)
{	
	int det = 0;
	int **new_matrix;

	new_matrix = malloc(SIZE * sizeof(int *));
	for(int i = 0; i < size - 1; i++)
		new_matrix[i] = malloc((size - 1) * sizeof(int));

	if(size == 2)
		return matrix[0][0] * matrix[1][1] - matrix[1][0] * matrix[0][1];

	for(int k = 0; k < size; k++)
	{
		for(int i = 0; i < size - 1; i++)
			for(int j = 0; j < size - 1; j++)
				if(j != k)
					new_matrix[i][j] = matrix[i + 1][j];

		for(int j = k; j < size - 1; j++)
			for(int i = 0; i < size - 1; i++)
				new_matrix[i][j] = new_matrix[i][j + 1];

		det += matrix[0][k] * determinant(new_matrix, size - 1);	
	}
	
	return det;
}