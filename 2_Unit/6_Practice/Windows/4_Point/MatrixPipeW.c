//Program that uses process inter communication to process two matrices
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <windows.h>

#define SIZE 10
#define BUF_SIZE 1000 

bool inverse(int A[SIZE][SIZE], float inverse[SIZE][SIZE]);
void adjoint(int A[SIZE][SIZE], int adj[SIZE][SIZE]);
void getCofactor(int A[SIZE][SIZE], int temp[SIZE][SIZE], int p, int q, int n);
int determinant(int A[SIZE][SIZE], int n);

int main(void)
{
	int i, j, offset;
	char *wbuffer, *rbuffer;
	int **matrix1, **matrix2, result_matrix[SIZE][SIZE];
	float inv[SIZE][SIZE];
	FILE *write_fp;
	DWORD written, read;
	HANDLE pipefd[2];
	PROCESS_INFORMATION piChild;
	STARTUPINFO siChild;
	SECURITY_ATTRIBUTES pipeSec = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE};

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
	//Obtencion de información para la inicialización del proceso hijo
	GetStartupInfo(&siChild);
	//Creacion de la tubería sin nombre
	CreatePipe(&pipefd[0], &pipefd[1], &pipeSec, 0);
	
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
	WriteFile(pipefd[1], wbuffer, strlen(wbuffer), &written, NULL);

	//Hereda el proceso hijo los manejadores de la tubería del proceso padre
	siChild.hStdInput = pipefd[0];
	siChild.hStdError = GetStdHandle(STD_ERROR_HANDLE);
	siChild.hStdOutput = pipefd[1];
	siChild.dwFlags = STARTF_USESTDHANDLES;
	if(!CreateProcess(NULL, "MatrixPipeWChild", NULL, NULL, TRUE, 0, NULL, NULL, &siChild, &piChild))
		fprintf(stderr, "Error\n");

	//Read from first pipe result of multiplication and sum
	//Wait necessary this time
	WaitForSingleObject(piChild.hProcess, INFINITE);
	ReadFile(pipefd[0], rbuffer, BUF_SIZE, &read, NULL);
	
	//Result of sum always is written first on the pipe at least on Windows	
	//Read into result_matrix the result of multiplication from rbuffer
	offset = 0;
	for(i = 0; i < SIZE; i++)
		for(j = 0; j < SIZE; j++)
		{
			sscanf(rbuffer, " %d%n", &result_matrix[i][j], &offset);
			rbuffer += offset;
		}
	//Calculate inverse of multiplication and send it to a file
	write_fp = fopen("SumInv.txt", "w");
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
	write_fp = fopen("MultiplicationInv.txt", "w");	
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

	CloseHandle(pipefd[0]);
	CloseHandle(pipefd[1]);
	CloseHandle(piChild.hThread);
	CloseHandle(piChild.hProcess);
		
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

