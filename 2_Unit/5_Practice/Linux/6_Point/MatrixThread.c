#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>

#define SIZE 10

typedef unsigned char BYTE;

int determinant(int **matrix, int size);
void *sum(void *arg);
void *subtraction(void *arg);
void *multiplication(void *arg);
void *transpose(void *arg);
void *inverse(void *arg);

typedef struct 
{
	int **matrix1;
	int **matrix2;
}Matrices;

int main(void)
{
	pid_t son_pid;
	pthread_t id_thread[5];
	BYTE i = 0, j = 0, k = 0, l = 0, m = 0;
	int **matrix1, **matrix2, result_matrix[SIZE][SIZE];
	clock_t begin, end;
	FILE *write_fp, *read_fp;	

	Matrices argv;
	Matrices *argvp;

	//Create the matrices
	matrix1 = malloc(SIZE * sizeof(int *));
	for(i = 0; i < SIZE; i++)
		matrix1[i] = malloc(SIZE * sizeof(int));

	matrix2 = malloc(SIZE * sizeof(int *));
	for(i = 0; i < SIZE; i++)
		matrix2[i] = malloc(SIZE * sizeof(int));

	//Fill the matrices
	srand((unsigned) time(NULL));
	for(i = 0; i < SIZE; i++)
		for(j = 0; j < SIZE; j++)
		{
			matrix1[i][j] = rand() % 10;
			matrix2[i][j] = rand() % 10;
		}

	//Copy the reference of the two matrices into the struct argv for further argument to the threads
	argv.matrix1 = matrix1;
	argv.matrix2 = matrix2;

	//Copy the reference of the struct into argvp
	argvp = &argv;

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
	//Call 5 processes to run
	for(i = 0; i < 5; i++)
	{
		switch(i)
		{
			case 0:
				pthread_create(&id_thread[0], NULL, sum, argvp);
			break;
			case 1:
				pthread_create(&id_thread[1], NULL, subtraction, argvp);
			break;
			case 2:
				pthread_create(&id_thread[2], NULL, multiplication, argvp);
			break;
			case 3:
				pthread_create(&id_thread[3], NULL, transpose, argvp);
			break;
			case 4:
				pthread_create(&id_thread[4], NULL, inverse, argvp);
			break;
		}
	}
	for(i = 0; i < 5; i++)
		pthread_join(id_thread[i], NULL);
	printf("THREAD 6\n");

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
			printf("%5d ", matrix1[i][j]);
		printf("\n");
	}
	printf("\n\t\t\tINVERSE OF MATRIX 2\n");
	for(i = 0; i < SIZE; i++)
	{
		for(j = 0; j < SIZE; j++)
			printf("%5d ", matrix2[i][j]);
		printf("\n");
	}
	end = clock();

	printf("\nExecution time: %f seconds\n", (double) (end - begin) / CLOCKS_PER_SEC);

	//Finally terminate main process
	return 0;
}

void *sum(void *arg)
{
	Matrices *argv = arg;	
	FILE *write_fp, *read_fp;
	int matrix1[SIZE][SIZE], matrix2[SIZE][SIZE];

	for(int i = 0; i < SIZE; i++)
		for(int j = 0;j < SIZE; j++)
		{
			matrix1[i][j] = argv->matrix1[i][j];
			matrix2[i][j] = argv->matrix2[i][j];
		}

	printf("SUM\n");	
	write_fp = fopen("Sum.txt", "w");
	for(int j = 0; j < SIZE; j++)
		for(int k = 0; k < SIZE; k++)
			matrix1[j][k] = matrix1[j][k] + matrix2[j][k];

	for(int j = 0; j < SIZE; j++)
	{
		for(int k = 0; k < SIZE; k++)
			fprintf(write_fp, "%d ", matrix1[j][k]);
		fprintf(write_fp, "\n");
	}
	fclose(write_fp);

}

void *subtraction(void *arg)
{
	Matrices *argv = arg;	
	int matrix1[SIZE][SIZE], matrix2[SIZE][SIZE];
	FILE *write_fp, *read_fp;

	for(int i = 0; i < SIZE; i++)
		for(int j = 0;j < SIZE; j++)
		{
			matrix1[i][j] = argv->matrix1[i][j];
			matrix2[i][j] = argv->matrix2[i][j];
		}

	printf("SUBTRACTION\n");
	write_fp = fopen("Subtraction.txt", "w");
	for(int j = 0; j < SIZE; j++)
		for(int k = 0; k < SIZE; k++)
			matrix1[j][k] = matrix1[j][k] - matrix2[j][k];

	for(int j = 0; j < SIZE; j++)
	{
		for(int k = 0; k < SIZE; k++)
			fprintf(write_fp, "%d ", matrix1[j][k]);
		fprintf(write_fp, "\n");
	}
	fclose(write_fp);
}

void *multiplication(void *arg)
{
	Matrices *argv = arg;	
	int accumulator = 0, result_matrix[SIZE][SIZE];
	int matrix1[SIZE][SIZE], matrix2[SIZE][SIZE];
	FILE *write_fp, *read_fp;

	for(int i = 0; i < SIZE; i++)
		for(int j = 0;j < SIZE; j++)
		{
			matrix1[i][j] = argv->matrix1[i][j];
			matrix2[i][j] = argv->matrix2[i][j];
		}

	printf("MULTIPLICATION\n");
	write_fp = fopen("Multiplication.txt", "w");
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

	for(int j = 0; j < SIZE; j++)
	{
		for(int k = 0; k < SIZE; k++)
			fprintf(write_fp, "%d ", result_matrix[j][k]);
		fprintf(write_fp, "\n");
	}
	fclose(write_fp);
}

void *transpose(void *arg)
{
	Matrices *argv = arg;
	int matrix1[SIZE][SIZE], matrix2[SIZE][SIZE];
	FILE *write_fp, *read_fp;

	for(int i = 0; i < SIZE; i++)
		for(int j = 0;j < SIZE; j++)
		{
			matrix1[i][j] = argv->matrix1[i][j];
			matrix2[i][j] = argv->matrix2[i][j];
		}
	
	printf("TRANSPOSE\n");
	write_fp = fopen("Transpose.txt", "w");
	for(int j = 0; j < SIZE; j++)
		for(int k = 0; k < SIZE; k++)
			matrix1[j][k] = matrix1[k][j];

	for(int j = 0; j < SIZE; j++)
		for(int k = 0; k < SIZE; k++)
			matrix2[j][k] = matrix2[k][j];	

	for(int j = 0; j < SIZE; j++)
	{
		for(int k = 0; k < SIZE; k++)
			fprintf(write_fp, "%d ", matrix1[j][k]);
		fprintf(write_fp, "\n");
	}

	fprintf(write_fp, "\n");
	for(int j = 0; j < SIZE; j++)
	{
		for(int k = 0; k < SIZE; k++)
			fprintf(write_fp, "%d ", matrix2[j][k]);
		fprintf(write_fp, "\n");
	}
	fclose(write_fp);
}

void *inverse(void *arg)
{
	Matrices *argv = arg;	
	int det = 0;
	FILE *write_fp, *read_fp;
	int **matrix1, **matrix2;

	//Create the matrices
	matrix1 = malloc(SIZE * sizeof(int *));
	for(int i = 0; i < SIZE; i++)
		matrix1[i] = malloc(SIZE * sizeof(int));

	matrix2 = malloc(SIZE * sizeof(int *));
	for(int i = 0; i < SIZE; i++)
		matrix2[i] = malloc(SIZE * sizeof(int));

	for(int i = 0; i < SIZE; i++)
		for(int j = 0;j < SIZE; j++)
		{
			matrix1[i][j] = argv->matrix1[i][j];
			matrix2[i][j] = argv->matrix2[i][j];
		}
	
	printf("INVERSE\n");
	write_fp = fopen("Inverse.txt", "w");

	//Calculate the determinant of matrix1
	det = determinant(matrix1, SIZE);

	for(int j = 0; j < SIZE; j++)
	{
		for(int k = 0; k < SIZE; k++)
			fprintf(write_fp, "%d ",det * argv->matrix1[j][k]);
		fprintf(write_fp, "\n");
	}

	//Calculate the determinant of matrix2
	det = determinant(matrix2, SIZE);
	fprintf(write_fp, "\n");
	for(int j = 0; j < SIZE; j++)
	{
		for(int k = 0; k < SIZE; k++)
			fprintf(write_fp, "%d ", det * argv->matrix2[j][k]);
		fprintf(write_fp, "\n");
	}

	fclose(write_fp);
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
