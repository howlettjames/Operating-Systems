#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>

void *copyDir(void *arg);

typedef struct 
{
	char source_path[FILENAME_MAX];
	char dest_path[FILENAME_MAX];
}Paths;

int main(int argc, char *argv[])
{
	pthread_t id_thread;
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
	pthread_create(&id_thread, NULL, copyDir, arg);
	pthread_join(id_thread, NULL);

	return 0;
}

void *copyDir(void *arg)
{
	Paths *dir_path = arg;
	char source_path[FILENAME_MAX], dest_path[FILENAME_MAX];
	struct dirent **namelist;
	struct stat stat_buf;
	pthread_t id_thread;
	int n, i, j, fd, chunk_size, fd1;
	char buffer[100], dest_path_fname[FILENAME_MAX];
	Paths paths;
	Paths *argv;

	//Copy the source and dest path into the new source and dest paths
	strcpy(source_path, dir_path->source_path);
	strcpy(dest_path, dir_path->dest_path);

	//Scan the source directory and check if it could be scanned	
	n = scandir(source_path, &namelist, NULL, alphasort);
	if(n == -1)
	{
		printf("Error: cannot scan source directory\n");
		exit(EXIT_FAILURE);
	}

	//Start at 2 because the first two entries are "." and ".." always
	for(j = 2; j < n; j++)
	{
		//Change to source directory to be able to use relative pathnames
		if((chdir(source_path)) == -1)
		{
			printf("Error: cannot open source directory\n");
			exit(EXIT_FAILURE);
		}
		printf("%s\n", namelist[j]->d_name);
		if((stat(namelist[j]->d_name, &stat_buf)) == -1)
		{
			printf("Error: cannot check \"%s\" file status\n", namelist[j]->d_name);
			exit(EXIT_FAILURE);
		}

		//Check whether the entry is a regular file or directory
		if(S_ISREG(stat_buf.st_mode))
		{
			//printf("-Regular file\n");
			//Open the file to read from
			if((fd = open(namelist[j]->d_name, O_RDONLY)) == -1)
			{
				printf("Error: cannot open \"%s\"\n", namelist[j]->d_name);
				exit(EXIT_FAILURE);
			}
			//Clear the dest_path_fname and fill it with the pathname of the new file to create
			for(i = 0; i < FILENAME_MAX; i++)
				dest_path_fname[i] = '\0';
			strcpy(dest_path_fname, dest_path);
			strcat(dest_path_fname, "/");
			strcat(dest_path_fname, namelist[j]->d_name);

			//Create the file to write to
			if((fd1 = creat(dest_path_fname, S_IRWXU)) == -1)
			{
				printf("Error: cannot create \"%s\"\n", dest_path_fname);
				exit(EXIT_FAILURE);
			}
			//Read chunks of 100 bytes from the file and write them to the file destination until there's no longer more bytes to read
			while((chunk_size = read(fd, buffer, 100)) > 0)
			{
				/*for(i = 0; i < chunk_size; i++)
					printf("%c", buffer[i]);
				printf("\n");*/
				write(fd1, buffer, chunk_size);
			}
			close(fd1);
			close(fd);	
		}
		else if(S_ISDIR(stat_buf.st_mode))
		{
			//printf("-Directory\n");		
			//Clear the dest_path_fname and fill it with the path name of the new directory to create
			for(i = 0; i < FILENAME_MAX; i++)
				dest_path_fname[i] = '\0';
			strcpy(dest_path_fname, dest_path);
			strcat(dest_path_fname, "/");
			strcat(dest_path_fname, namelist[j]->d_name);

			if((mkdir(dest_path_fname, S_IRWXU)) == -1)
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
			strcat(dest_path_fname, namelist[j]->d_name);
			//Save the paths of the source and destination paths into the structure
			strcpy(paths.source_path, dest_path_fname);
			
			//Make a pointer to the structure
			argv = &paths;
			//Call the next thread
			pthread_create(&id_thread, NULL, copyDir, argv);
			//pthread_join(id_thread, NULL);
		}
	}
	return NULL;
}