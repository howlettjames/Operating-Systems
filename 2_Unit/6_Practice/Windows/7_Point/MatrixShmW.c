//Program that uses process inter communication to process two matrices
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <windows.h>

#define SIZE 10
#define MEM_SIZE 5000 

bool inverse(int A[SIZE][SIZE], float inverse[SIZE][SIZE]);
void adjoint(int A[SIZE][SIZE], int adj[SIZE][SIZE]);
void getCofactor(int A[SIZE][SIZE], int temp[SIZE][SIZE], int p, int q, int n);
int determinant(int A[SIZE][SIZE], int n);

int main(void)
{
	int i, j, k;
	int *buffer, *shm;
	int **matrix1, **matrix2, result_matrix[SIZE][SIZE];
	float inv[SIZE][SIZE];
	char *shmid = "SharedMemory";
	HANDLE hMapFile;
	FILE *write_fp;
	PROCESS_INFORMATION piChild;
	STARTUPINFO siChild;

	//Don't know what for but necessary to create child process correctly
	ZeroMemory(&siChild, sizeof(siChild));
	siChild.cb = sizeof(siChild);
	ZeroMemory(&piChild, sizeof(piChild));

	srand((unsigned) time(NULL));

	//Asign memory space for both matrices and buffers
	matrix1 = malloc(SIZE * sizeof(int *));
	matrix2 = malloc(SIZE * sizeof(int *));
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

	//Map shared memory
	if((hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, MEM_SIZE, shmid)) == NULL)
	{
		printf("Failed to map shared memory: (%i)\n", GetLastError());
		exit(EXIT_FAILURE);
	}
	//Create shared memory
	if((shm = (int *)MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, MEM_SIZE)) == NULL)
	{
		printf("Failed to create shared memory: (%i)\n", GetLastError());
		CloseHandle(hMapFile);
		exit(EXIT_FAILURE); 
	}
	//Make a pointer to the shared memory named buffer
	buffer = shm;

	//Write both matrices to the shared memory
	k = 0;
	for(i = 0; i < SIZE; i++)
		for(j = 0; j < SIZE; j++)
			buffer[k++] = matrix1[i][j];

	for(i = 0; i < SIZE; i++)
		for(j = 0; j < SIZE; j++)
			buffer[k++] = matrix2[i][j];

	//Put a marker to indicate end of matrices
	buffer[k] = -1000;	
	
	//Create child process
	if(!CreateProcess(NULL, "MatrixShmWChild", NULL, NULL, FALSE, 0, NULL, NULL, &siChild, &piChild))
	{
		printf("Failed to create child process\n");
		exit(EXIT_FAILURE);
	}

	//Wait until sum is written to the shared memory
	k = 4 * SIZE * SIZE;
	while(buffer[k] != -3000)
		Sleep(1000);
	/*
	//Print all the content into shared memory
	for(i = 0; i < SIZE * SIZE; i++)
	{
		if(i % 10 == 0 && i != 0)
			printf("\n");	
		printf("%d ", buffer[i]);
	}
	printf("\n");
	for(; i < 2 * SIZE * SIZE; i++)
	{
		if(i % 10 == 0 && i != 0)
			printf("\n");	
		printf("%d ", buffer[i]);
	}
	printf("\n");

	for(; i < 3 * SIZE * SIZE; i++)
	{
		if(i % 10 == 0 && i != 0)
			printf("\n");	
		printf("%d ", buffer[i]);
	}
	printf("\n");

	for(; i < 4 * SIZE * SIZE; i++)
	{
		if(i % 10 == 0 && i != 0)
			printf("\n");	
		printf("%d ", buffer[i]);
	}
	printf("\n");
	*/
	
	//Read into result_matrix the result of multiplication from shared memory
	k = 2 * SIZE * SIZE;
	for(i = 0; i < SIZE; i++)
		for(j = 0; j < SIZE; j++)
			result_matrix[i][j] = buffer[k++];

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

	//Read into result_matrix the result of sum from shared memory
	k = 3 * SIZE * SIZE;
	for(i = 0; i < SIZE; i++)
		for(j = 0; j < SIZE; j++)
			result_matrix[i][j] = buffer[k++];

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
	
	CloseHandle(piChild.hThread);
	CloseHandle(piChild.hProcess);
	UnmapViewOfFile(shm);
	CloseHandle(hMapFile);
	
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

