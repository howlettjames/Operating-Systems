#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define SIZE 10

typedef unsigned char BYTE;

int determinant(int **matrix, int size);

int main(void)
{
	pid_t son_pid;
	BYTE i = 0, j = 0, k = 0, l = 0, m = 0;
	int **matrix1, **matrix2, result_matrix[SIZE][SIZE];
	int wstatus, accumulator, det, result;
	clock_t begin, end;
	FILE *write_fp, *read_fp;

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
	{
		son_pid = fork();
		if(son_pid != 0)
			printf("Called process no.%d\n", i + 1);
		else
		{
			printf("Process %d\n", i + 1);
			switch(i)
			{
				case 0:
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
				break;
				case 1:
					printf("SUBTRACTION\n");
					write_fp = fopen("Subtraction.txt", "w");
					for(j = 0; j < SIZE; j++)
						for(k = 0; k < SIZE; k++)
							matrix1[j][k] = matrix1[j][k] - matrix2[j][k];

					for(j = 0; j < SIZE; j++)
					{
						for(k = 0; k < SIZE; k++)
						{
							fprintf(write_fp, "%d ", matrix1[j][k]);
						}
						fprintf(write_fp, "\n");
					}
					fclose(write_fp);
				break;
				case 2:
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
				break;
				case 3:
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
				break;
				case 4:
					printf("INVERSE\n");
					write_fp = fopen("Inverse.txt", "w");
					//Calculate the determinant of matrix1
					det = determinant(matrix1, SIZE);

					for(j = 0; j < SIZE; j++)
					{
						for(k = 0; k < SIZE; k++)
						{
							fprintf(write_fp, "%d ",det * matrix1[j][k]);
						}
						fprintf(write_fp, "\n");
					}

					//Calculate the determinant of matrix2
					det = determinant(matrix2, SIZE);
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
				break;
			}
			//Terminate the current process
			exit(EXIT_SUCCESS);
		}
	}
	//Wait until the last process called is terminated, then execute the process no.6 
	//which reads the results saved in the respectives files by the first 5 processes
	waitpid(son_pid, &wstatus, 0);
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
	end = clock();

	printf("\nExecution time: %f seconds\n", (double) (end - begin) / CLOCKS_PER_SEC);

	//Finally terminate main process
	exit(EXIT_SUCCESS);
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