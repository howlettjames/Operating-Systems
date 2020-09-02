//Program that uses process inter communication to process two matrices
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define SIZE 10
#define BUF_SIZE 1000 

bool inverse(int A[SIZE][SIZE], float inverse[SIZE][SIZE]);
void adjoint(int A[SIZE][SIZE], int adj[SIZE][SIZE]);
void getCofactor(int A[SIZE][SIZE], int temp[SIZE][SIZE], int p, int q, int n);
int determinant(int A[SIZE][SIZE], int n);

int main(void)
{
	int pipefd[2], i, j, offset;
	char *wbuffer, *rbuffer;
	int **matrix1, **matrix2, result_matrix[SIZE][SIZE];
	float inv[SIZE][SIZE];
	FILE *write_fp;

	srand((unsigned) time(NULL));

	//Asign memory space for both matrices and buffers
	matrix1 = malloc(SIZE * sizeof(int *));
	matrix2 = malloc(SIZE * sizeof(int *));
	wbuffer = malloc(BUF_SIZE);
	rbuffer = malloc(BUF_SIZE);
	for(i = 0; i < SIZE; i++)
	{
		matrix1[i] = malloc(SIZE * sizeof(int));
		matrix2[i] = malloc(SIZE * sizeof(int));
	}

	//Fill the matrices with random values
	for(i = 0; i < SIZE; i++)
		for(j = 0; j < SIZE; j++)
		{
			matrix1[i][j] = rand() % 5;
			matrix2[i][j] = rand() % 5;
		}	

	//Create the first pipe
	if(pipe(pipefd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}	

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
	//Write matrix1 and matrix2 to the first pipe
	write(pipefd[1], wbuffer, strlen(wbuffer));

	//Create child process
	if(fork() == 0)
	{
		int matrix1[SIZE][SIZE], matrix2[SIZE][SIZE], result_matrix[SIZE][SIZE], i, j, offset = 0, pipefd1[2], accumulator;
		char *wbuffer, *rbuffer;

		//Allocate memory for buffers
		wbuffer = malloc(BUF_SIZE);
		rbuffer = malloc(BUF_SIZE);

		//Read matrix1 and matrix2 from first pipe
		read(pipefd[0], rbuffer, BUF_SIZE);

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
		if(pipe(pipefd1) == -1)
		{
			perror("pipe");
			exit(EXIT_FAILURE);
		}

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
		write(pipefd1[1], wbuffer, strlen(wbuffer));	

		//Create child process
		if(fork() == 0)	
		{
			//Close unused write end
			close(pipefd1[1]);

			int matrix1[SIZE][SIZE], matrix2[SIZE][SIZE], result_matrix[SIZE][SIZE], i, j, offset = 0;		
			char *wbuffer, *rbuffer;

			//Allocate memory for buffers
			wbuffer = malloc(BUF_SIZE);
			rbuffer = malloc(BUF_SIZE);
		
			//Read matrix1 and matrix2 from pipe
			read(pipefd1[0], rbuffer, BUF_SIZE);

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
			write(pipefd[1], wbuffer, strlen(wbuffer));		

			exit(EXIT_SUCCESS);
		}	
		else
		{
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

			for(i = 0; i < SIZE; i++)
			{
				for(j = 0; j < SIZE; j++)
					printf("%d ", result_matrix[i][j]);
				printf("\n");
			}
			printf("\n");
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
			write(pipefd[1], wbuffer, strlen(wbuffer));
		}
		wait(0);
		exit(EXIT_SUCCESS);
	}
	//Father
	else
	{	
		//Read from first pipe result of multiplication and sum
		//Wait necessary in order to read the results from both child processes
		wait(0);
		read(pipefd[0], rbuffer, BUF_SIZE);

		//In this case the multiplication is always written first in the pipe not as it happens on Windows
		//Read into result_matrix the result of multiplication from rbuffer
		offset = 0;
		for(i = 0; i < SIZE; i++)
			for(j = 0; j < SIZE; j++)
			{
				sscanf(rbuffer, " %d%n", &result_matrix[i][j], &offset);
				rbuffer += offset;
			}
		//Calculate inverse of multiplication and send it to a file
		write_fp = fopen("MultiplicationInv.txt", "w");
		printf("Sending inverse of multiplication to a file...\n");
		if(inverse(result_matrix, inv))
		{
			for(i = 0; i < SIZE; i++)
			{
				for(j = 0; j < SIZE; j++)
					fprintf(write_fp, "%6.2f ", inv[i][j]);
				fprintf(write_fp, "\n");
			}
		}
		fclose(write_fp);

		//Read into result_matrix the result of sum from rbuffer
		for(i = 0; i < SIZE; i++)
			for(j = 0; j < SIZE; j++)
			{
				sscanf(rbuffer, " %d%n", &result_matrix[i][j], &offset);
				rbuffer += offset;
			}
		//Calculate inverse of sum and send it to a file
		write_fp = fopen("SumInv.txt", "w");	
		printf("Sending inverse of sum to a file...\n");
		if(inverse(result_matrix, inv))
		{
			for(i = 0; i < SIZE; i++)
			{
				for(j = 0; j < SIZE; j++)
					fprintf(write_fp, "%6.2f ", inv[i][j]);
				fprintf(write_fp, "\n");
			}
		}
		fclose(write_fp);	
	}
	
	return 0;
}

// Function to calculate and store inverse, returns false if
// matrix is singular
bool inverse(int A[SIZE][SIZE], float inverse[SIZE][SIZE])
{
    // Find determinant of A[][]
    int det = determinant(A, SIZE);
    if (det == 0)
    {
        printf("Singular matrix, can't find its inverse\n");
        return false;
    }
 
    // Find adjoint
    int adj[SIZE][SIZE];
    adjoint(A, adj);
 
    // Find Inverse using formula "inverse(A) = adj(A)/det(A)"
    for (int i=0; i<SIZE; i++)
        for (int j=0; j<SIZE; j++)
            inverse[i][j] = adj[i][j]/(float) det;
 
    return true;
}

// Function to get adjoint of A[N][N] in adj[N][N]. 
void adjoint(int A[SIZE][SIZE], int adj[SIZE][SIZE])
{
    if (SIZE == 1)
    {
        adj[0][0] = 1;
        return;
    }
 
    // temp is used to store cofactors of A[][]
    int sign = 1, temp[SIZE][SIZE];
 
    for (int i=0; i<SIZE; i++)
    {
        for (int j=0; j<SIZE; j++)
        {
            // Get cofactor of A[i][j]
            getCofactor(A, temp, i, j, SIZE);
 
            // sign of adj[j][i] positive if sum of row
            // and column indexes is even.
            sign = ((i+j)%2==0)? 1: -1;
 
            // Interchanging rows and columns to get the
            // transpose of the cofactor matrix
            adj[j][i] = (sign)*(determinant(temp, SIZE-1));
        }
    }
}

// Function to get cofactor of A[p][q] in temp[][]. n is current
// dimension of A[][]
void getCofactor(int A[SIZE][SIZE], int temp[SIZE][SIZE], int p, int q, int n)
{
    int i = 0, j = 0;
 
    // Looping for each element of the matrix
    for (int row = 0; row < n; row++)
    {
        for (int col = 0; col < n; col++)
        {
            //  Copying into temporary matrix only those element
            //  which are not in given row and column
            if (row != p && col != q)
            {
                temp[i][j++] = A[row][col];
 
                // Row is filled, so increase row index and
                // reset col index
                if (j == n - 1)
                {
                    j = 0;
                    i++;
                }
            }
        }
    }
}

/* Recursive function for finding determinant of matrix.
   n is current dimension of A[][]. */
int determinant(int A[SIZE][SIZE], int n)
{
    int D = 0; // Initialize result
 
    //  Base case : if matrix contains single element
    if (n == 1)
        return A[0][0];
 
    int temp[SIZE][SIZE]; // To store cofactors
 
    int sign = 1;  // To store sign multiplier
 
     // Iterate for each element of first row
    for (int f = 0; f < n; f++)
    {
        // Getting Cofactor of A[0][f]
        getCofactor(A, temp, 0, f, n);
        D += sign * A[0][f] * determinant(temp, n - 1);
 
        // terms are to be added with alternate sign
        sign = -sign;
    }
 
    return D;
}

