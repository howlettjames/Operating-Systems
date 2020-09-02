//Program that uses process inter communication to process two matrices with semaphores
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define SIZE 10
#define MEM_SIZE 5000 

bool inverse(int A[SIZE][SIZE], float inverse[SIZE][SIZE]);
void adjoint(int A[SIZE][SIZE], int adj[SIZE][SIZE]);
void getCofactor(int A[SIZE][SIZE], int temp[SIZE][SIZE], int p, int q, int n);
int determinant(int A[SIZE][SIZE], int n);

int main(void)
{
	int i, j, k;
	pid_t cpid;
	int *buffer, shmid, *shm, semid, nsems = 2, semflg = IPC_CREAT | 0666, nsops;
	int **matrix1, **matrix2, result_matrix[SIZE][SIZE];
	float inv[SIZE][SIZE];
	key_t sem_key = 1234;
	struct sembuf *sops = malloc(4 * sizeof(struct sembuf));
	FILE *write_fp;

	srand((unsigned) time(NULL));

	//Asign memory space for both matrices
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

	//ftok to generate an unique key
	key_t key = 2018;
	//shmget returns an identifier in shmid
	if((shmid = shmget(key, MEM_SIZE, 0666 | IPC_CREAT)) < 0)
	{
		perror("Failed to get shared memory: shmget");
		exit(EXIT_FAILURE);
	}
	//shmat to attach to shared memory
	if((shm = shmat(shmid, NULL, 0)) == (int *) -1)
	{
		perror("Failed to attach to shared memory: shmat");
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

	//Creating the semaphore
	if((semid = semget(sem_key, nsems, semflg)) == -1)
	{
		perror("semget: failed to get semaphore");
		exit(1);
	}	
	//Create child process
	if(fork() == 0)
	{
		int matrix1[SIZE][SIZE], matrix2[SIZE][SIZE], result_matrix[SIZE][SIZE], i, j, k, accumulator;

		//Create child process
		if((cpid = fork()) == 0)	
		{
			int matrix1[SIZE][SIZE], matrix2[SIZE][SIZE], result_matrix[SIZE][SIZE], i, j, k;		

			//Copy into the matrices the values stored into shared memory
			k = 0;
			for(i = 0; i < SIZE; i++)
				for(j = 0; j < SIZE; j++)
					matrix1[i][j] = buffer[k++];

			for(i = 0; i < SIZE; i++)
				for(j = 0; j < SIZE; j++)
					matrix2[i][j] = buffer[k++];

			//Do sum
			for(i = 0; i < SIZE; i++)
				for(j = 0; j < SIZE; j++)
					result_matrix[i][j] = matrix1[i][j] + matrix2[i][j];	

			//Halt until Multiplication is done	
			sleep(1);

			nsops = 1;
			sops[0].sem_num = 1;
			sops[0].sem_op = 0; //Wait for semaphore to become 0
			sops[0].sem_flg = SEM_UNDO; //Take off semaphore

			if((semop(semid, sops, nsops)) == -1)
			{
				perror("semop: failed to do operations over the semaphoreE\n");
				exit(EXIT_FAILURE);
			}
			//printf("B\n");
			//AT THIS POINT THIS PROCESS HAS THE CONTROL
			//Write sum into shared memory starting at 3 * SIZE * SIZE
			k = 3 * SIZE * SIZE;
			for(i = 0; i < SIZE; i++)
				for(j = 0; j < SIZE; j++)
					buffer[k++] = result_matrix[i][j];			

			nsops = 1;
			sops[0].sem_num = 0;
			sops[0].sem_op = -1; //Give up control by subtracting 1 to the semaphore thus making it 0
			sops[0].sem_flg = SEM_UNDO | IPC_NOWAIT; //Take off semaphore
			
			if((semop(semid, sops, nsops)) == -1)
			{
				perror("semop: failed to do operations over the semaphoreD\n");
				exit(EXIT_FAILURE);
			}
			exit(EXIT_SUCCESS);
		}	
		else
		{	
			nsops = 2;
			
			sops[0].sem_num = 0;
			sops[0].sem_op = 1; //Increment semaphore by 1, take control
			sops[0].sem_flg = SEM_UNDO | IPC_NOWAIT; //Take off semaphore

			sops[1].sem_num = 1;
			sops[1].sem_op = 1; //Increment semaphore by 1, take control
			sops[1].sem_flg = SEM_UNDO | IPC_NOWAIT; //Take off semaphore

			if((semop(semid, sops, nsops)) == -1)
			{
				perror("semop: failed to do operations over the semaphoreB\n");
				exit(EXIT_FAILURE);
			}
			//AT THIS POINT THIS PROCESS HAS THE CONTROL
			//printf("A\n");
			//Copy into the matrices the values stored into shared memory
			k = 0;
			for(i = 0; i < SIZE; i++)
				for(j = 0; j < SIZE; j++)
					matrix1[i][j] = buffer[k++];

			for(i = 0; i < SIZE; i++)
				for(j = 0; j < SIZE; j++)
					matrix2[i][j] = buffer[k++];

			//Do multiplication
			for(j = 0; j < SIZE; j++)
			{
				for(k = 0; k < SIZE; k++)
				{
					accumulator = 0;
					for(int l = 0, m = 0; l < SIZE && m < SIZE; l++, m++)
						accumulator += matrix1[j][l] * matrix2[m][k];
					result_matrix[j][k] = accumulator;
				}
			}
			//Write multiplication into shared memory starting at 2 * SIZE * SIZE
			k = 2 * SIZE * SIZE;
			for(i = 0; i < SIZE; i++)
				for(j = 0; j < SIZE; j++)
					buffer[k++] = result_matrix[i][j];			

			nsops = 1;
			sops[0].sem_num = 1;
			sops[0].sem_op = -1; //Give up control by subtracting 1 to the semaphore thus making it 0
			sops[0].sem_flg = SEM_UNDO | IPC_NOWAIT; //Take off semaphore

			if((semop(semid, sops, nsops)) == -1)
			{
				perror("semop: failed to do operations over the semaphoreC\n");
				exit(EXIT_FAILURE);
			}
			//Wait until grand child is done to not leave abandoned sempahore 0
			waitpid(cpid, NULL, 0);
		}
		exit(EXIT_SUCCESS);
	}
	//Father
	else
	{	
		sleep(1);

		nsops = 1;
		sops[0].sem_num = 0;
		sops[0].sem_op = 0; //Wait for semaphore to become 0
		sops[0].sem_flg = SEM_UNDO; //Take off semaphore

		if((semop(semid, sops, nsops)) == -1)
		{
			perror("semop: failed to do operations over the semaphoreA\n");
			exit(EXIT_FAILURE);
		}
		//printf("C\n");
		
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
		
		//AT THIS POINT THE FATHER TAKES THE CONTROL
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

