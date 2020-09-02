#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <Windows.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdbool.h>

DWORD WINAPI copyDir(LPVOID arg);

typedef struct 
{
	char source_path[FILENAME_MAX];
	char dest_path[FILENAME_MAX];
}Paths;

int main(int argc, char *argv[])
{
	DWORD id_thread;
	HANDLE han_thread;
	Paths paths;
	Paths *arg;

	printf("\n");
	//Test if the call of the program was done right
	if(argc != 3)
	{
		printf("Usage: «program_name» «source_directory» «destination_directory»\n");
		exit(EXIT_FAILURE);	
	}

	//Save the paths of the source and destination paths into the structure
	strcpy(paths.source_path, argv[1]);
	strcpy(paths.dest_path, argv[2]);
	//Make a pointer to the structure
	arg = &paths;

	//Call the first thread with the pointer as argument to the thread that will use the paths
	han_thread = CreateThread(NULL, 0, copyDir, arg, 0, &id_thread);
	WaitForSingleObject(han_thread, INFINITE);

	return 0;
}

DWORD WINAPI copyDir(LPVOID arg)
{
	Paths *dir_path = arg;
	char source_path[FILENAME_MAX], dest_path[FILENAME_MAX];
	DWORD id_thread;
	HANDLE han_thread;
	int n, i, j;
	long unsigned int chunk_size;
	char buffer[100], dest_path_fname[FILENAME_MAX];
	Paths paths;
	Paths *argv;
	DIR *directory_stream;
	struct dirent *file_pointer;
	HANDLE fd, fd1;

	//Copy the source and dest path into the new source and dest paths
	strcpy(source_path, dir_path->source_path);
	strcpy(dest_path, dir_path->dest_path);

	//Open the source directory
	if((directory_stream = opendir(source_path)) == NULL)
	{
		printf("Error: cannot open the directory.\n");
		exit(EXIT_FAILURE);
	}

	//To avoid checking ".." entry
	file_pointer = readdir(directory_stream);
	//To avoid "." entry
	file_pointer = readdir(directory_stream);

	//Start at 2 because the first two entries are "." and ".." always
	while((file_pointer = readdir(directory_stream)) != NULL)
	{
		//Change to source directory to be able to use relative pathnames
		if((SetCurrentDirectory(source_path)) == -1)
		{
			printf("Error: cannot open source directory\n");
			exit(EXIT_FAILURE);
		}
		printf("%s\n", file_pointer->d_name);

		//Check whether the entry is a regular file or directory
		if(file_pointer->d_type == 0)
		{
			//printf("-Regular file\n");
			//Open the file to read from
			if((fd = CreateFile(file_pointer->d_name, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE)
			{
				printf("Error: cannot open \"%s\"\n", file_pointer->d_name);
				exit(EXIT_FAILURE);
			}
			//Clear the dest_path_fname and fill it with the pathname of the new file to create
			for(i = 0; i < FILENAME_MAX; i++)
				dest_path_fname[i] = '\0';
			strcpy(dest_path_fname, dest_path);
			strcat(dest_path_fname, "/");
			strcat(dest_path_fname, file_pointer->d_name);

			//Create the file to write to
			if((fd1 = CreateFile(dest_path_fname, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE)
			{
				printf("Error: cannot create \"%s\"\n", dest_path_fname);
				exit(EXIT_FAILURE);
			}
			//Read chunks of 100 bytes from the file and write them to the file destination until there's no longer more bytes to read
			while(1)
			{
				/*for(i = 0; i < chunk_size; i++)
					printf("%c", buffer[i]);
				printf("\n");*/
				ReadFile(fd, buffer, 100, &chunk_size, NULL);
				if(chunk_size == 0)
					break;
				WriteFile(fd1, buffer, chunk_size, NULL, NULL);
			}
			CloseHandle(fd1);
			CloseHandle(fd);
		}
		else if(file_pointer->d_type == 16)
		{
			//printf("-Directory\n");		
			//Clear the dest_path_fname and fill it with the path name of the new directory to create
			for(i = 0; i < FILENAME_MAX; i++)
				dest_path_fname[i] = '\0';
			strcpy(dest_path_fname, dest_path);
			strcat(dest_path_fname, "/");
			strcat(dest_path_fname, file_pointer->d_name);

			if((CreateDirectory(dest_path_fname, NULL)) == 0)
			{
				printf("Error: cannot create directory \"%s\"\n", dest_path_fname);
				exit(EXIT_FAILURE);
			}
			//Save the new destination path name into the member of the structure
			strcpy(paths.dest_path, dest_path_fname);
			//Clear the dest_path_fname and fill it with the path name of the new source path name
			for(i = 0; i < FILENAME_MAX; i++)
				dest_path_fname[i] = '\0';
			strcpy(dest_path_fname, source_path);
			strcat(dest_path_fname, "/");
			strcat(dest_path_fname, file_pointer->d_name);
			//Save the paths of the source and destination paths into the structure
			strcpy(paths.source_path, dest_path_fname);
			
			//Make a pointer to the structure
			argv = &paths;
			//Call the next thread
			han_thread = CreateThread(NULL, 0, copyDir, argv, 0, &id_thread);
			//WaitForSingleObject(han_thread, INFINITE);
		}
	}
		
	return 0;
}