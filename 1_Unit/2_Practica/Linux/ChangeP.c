#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
	DIR *directory_stream;
	struct dirent *directory_entry;
	char file_name[FILENAME_MAX];
	struct stat file_status;
	short option1, option2, option_correct2 = 1;

	//ABRIMOS EL DIRECTORIO QUE INGRESO EL USUSARIO AL CORRER EL PROGRAMA EN LA LÍNEA DE COMANDOS
	if((directory_stream = opendir(argv[1])) == NULL)
	{
		printf("Error: Cannot open the directory.\n");
		exit(EXIT_FAILURE);
	}

	//RECORREMOS EL DIRECTORIO E IMPRIMIMOS EL NOMBRE DE SUS CONTENIDOS
	printf("Choose one of the files below to change its permissions: \n"); 
	while((directory_entry = readdir(directory_stream)) != NULL)
		printf(">> %s\n", directory_entry->d_name);

	//LEEMOS EL NOMBRE DEL ARCHIVO EL USUARIO QUIERE CAMBIAR SUS PERMISOS
	scanf("%s", file_name);
	printf("\n");

	//CAMBIANDO DE DIRECTORIO PARA PODER LLAMAR STAT SIN NECESIDAD DE AGREGAR EL NOMBRE DEL ARCHIVO A LA RUTA DEL DIRECTORIO (PROCESO LIOSO)
	if((chdir(argv[1])) == -1)
	{
		printf("Error: Cannot change the directory.\n");
		exit(EXIT_FAILURE);	
	}

	//OBTENEMOS EL STATUS DEL ARCHIVO
	if((stat(file_name, &file_status)) == -1)
	{
		printf("Error: Cannot open file status.\n");
		exit(EXIT_FAILURE);
	}

	//IMPRIMIMOS INFORMACIÓN ACERCA DEL ARCHIVO
	printf("File type: ");
	switch(file_status.st_mode & S_IFMT)
	{
		case S_IFBLK: printf("block device\n"); break;
		case S_IFCHR: printf("character device\n"); break;
		case S_IFDIR: printf("directory\n"); break;
		case S_IFIFO: printf("FIFO/pipe\n"); break;
		case S_IFLNK: printf("symlink\n"); break;
		case S_IFREG: printf("regular file\n"); break;
		case S_IFSOCK: printf("socket\n"); break;
		default: printf("unknown?\n"); break;
	}
	printf("Mode: %lo (octal)\n", (unsigned long) file_status.st_mode);
	
	for(;;)
	{
		printf("\nWhich permissions would you like to change?\n");
		printf("1: User\n");
		printf("2: Group\n");
		printf("3: Others\n");
		printf("4: Exit\n");
		scanf("%hd", &option1);
		option_correct2 = 1;
		switch(option1)
		{
			case 1:
				while(option_correct2 == 1)
				{
					printf("\nWhich user permissions would you like to change?\n");
					printf("1: Set read, write and execute permissions\n");
					printf("2: Set read permission\n");
					printf("3: Set write permission\n");
					printf("4: Set execute permission\n");
					printf("5: Set read and write permissions\n");
					printf("6: Set read and execute permissions\n");
					printf("7: Set write and execute permissions\n");
					scanf("%hd", &option2);
					switch(option2)
					{
						case 1: 
							if((chmod(file_name, S_IRWXU)) == -1)
							{
								printf("Error: cannot change user requested permissions\n");
								break;
							}
							option_correct2 = 0;
						break;
						case 2: 
							if((chmod(file_name, S_IRUSR)) == -1)
							{
								printf("Error: cannot change user requested permissions\n");
								break;
							}
							option_correct2 = 0;
						break;
						case 3: 
							if((chmod(file_name, S_IWUSR)) == -1)
							{
								printf("Error: cannot change user requested permissions\n");
								break;
							}
							option_correct2 = 0;
						break;
						case 4: 
							if((chmod(file_name, S_IXUSR)) == -1)
							{
								printf("Error: cannot change user requested permissions\n");
								break;
							}
							option_correct2 = 0;
						break;
						case 5: 
							if((chmod(file_name, S_IRUSR | S_IWUSR)) == -1)
							{
								printf("Error: cannot change user requested permissions\n");
								break;
							}
							option_correct2 = 0;
						break;
						case 6: 
							if((chmod(file_name, S_IRUSR | S_IXUSR)) == -1)
							{
								printf("Error: cannot change user requested permissions\n");
								break;
							}
							option_correct2 = 0;
						break;
						case 7: 
							if((chmod(file_name, S_IWUSR | S_IXUSR)) == -1)
							{
								printf("Error: cannot change user requested permissions\n");
								break;
							}
							option_correct2 = 0;
						break;
						default: printf("Choose a correct option.\n"); break;
					}
					printf("Done!\n");
				}
			break;
			case 2:
				while(option_correct2 == 1)
				{
					printf("\nWhich group permissions would you like to change?\n");
					printf("1: Set read, write and execute permissions\n");
					printf("2: Set read permission\n");
					printf("3: Set write permission\n");
					printf("4: Set execute permission\n");
					printf("5: Set read and write permissions\n");
					printf("6: Set read and execute permissions\n");
					printf("7: Set write and execute permissions\n");
					scanf("%hd", &option2);
					switch(option2)
					{
						case 1: 
							if((chmod(file_name, S_IRWXG)) == -1)
							{
								printf("Error: cannot change user requested permissions\n");
								break;
							}
							option_correct2 = 0;
						break;
						case 2: 
							if((chmod(file_name, S_IRGRP)) == -1)
							{
								printf("Error: cannot change user requested permissions\n");
								break;
							}
							option_correct2 = 0;
						break;
						case 3: 
							if((chmod(file_name, S_IWGRP)) == -1)
							{
								printf("Error: cannot change user requested permissions\n");
								break;
							}
							option_correct2 = 0;
						break;
						case 4: 
							if((chmod(file_name, S_IXGRP)) == -1)
							{
								printf("Error: cannot change user requested permissions\n");
								break;
							}
							option_correct2 = 0;
						break;
						case 5: 
							if((chmod(file_name, S_IRGRP | S_IWGRP)) == -1)
							{
								printf("Error: cannot change user requested permissions\n");
								break;
							}
							option_correct2 = 0;
						break;
						case 6: 
							if((chmod(file_name, S_IRGRP | S_IXGRP)) == -1)
							{
								printf("Error: cannot change user requested permissions\n");
								break;
							}
							option_correct2 = 0;
						break;
						case 7: 
							if((chmod(file_name, S_IWGRP | S_IXGRP)) == -1)
							{
								printf("Error: cannot change user requested permissions\n");
								break;
							}
							option_correct2 = 0;
						break;
						default: printf("Choose a correct option.\n"); break;
					}
					printf("Done!\n");
				}
			break;
			case 3:
				while(option_correct2 == 1)
				{
					printf("\nWhich other permissions would you like to change?\n");
					printf("1: Set read, write and execute permissions\n");
					printf("2: Set read permission\n");
					printf("3: Set write permission\n");
					printf("4: Set execute permission\n");
					printf("5: Set read and write permissions\n");
					printf("6: Set read and execute permissions\n");
					printf("7: Set write and execute permissions\n");
					scanf("%hd", &option2);
					switch(option2)
					{
						case 1: 
							if((chmod(file_name, S_IRWXO)) == -1)
							{
								printf("Error: cannot change user requested permissions\n");
								break;
							}
							option_correct2 = 0;
						break;
						case 2: 
							if((chmod(file_name, S_IROTH)) == -1)
							{
								printf("Error: cannot change user requested permissions\n");
								break;
							}
							option_correct2 = 0;
						break;
						case 3: 
							if((chmod(file_name, S_IWOTH)) == -1)
							{
								printf("Error: cannot change user requested permissions\n");
								break;
							}
							option_correct2 = 0;
						break;
						case 4: 
							if((chmod(file_name, S_IXOTH)) == -1)
							{
								printf("Error: cannot change user requested permissions\n");
								break;
							}
							option_correct2 = 0;
						break;
						case 5: 
							if((chmod(file_name, S_IROTH | S_IWOTH)) == -1)
							{
								printf("Error: cannot change user requested permissions\n");
								break;
							}
							option_correct2 = 0;
						break;
						case 6: 
							if((chmod(file_name, S_IROTH | S_IXOTH)) == -1)
							{
								printf("Error: cannot change user requested permissions\n");
								break;
							}
							option_correct2 = 0;
						break;
						case 7: 
							if((chmod(file_name, S_IWOTH | S_IXOTH)) == -1)
							{
								printf("Error: cannot change user requested permissions\n");
								break;
							}
							option_correct2 = 0;
						break;
						default: printf("Choose a correct option.\n"); break;
					}
					printf("Done!\n");
				}
			break;
			case 4:
				exit(EXIT_SUCCESS);
			break;
			default:
				printf("Choose a correct option\n");
			break;	
		}
	}

	closedir(directory_stream);

	return 0;
}