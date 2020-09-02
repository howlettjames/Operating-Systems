#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <Windows.h>

int main(int argc, char *argv[])
{
	DIR *directory_stream;
	struct dirent *file_pointer;
	short option, no_cfiles, i, j = 0;
	char file_name[FILENAME_MAX], *file_buf, new_directory[FILENAME_MAX], **file_cnames, old_directory[FILENAME_MAX];
	HANDLE file_descriptor, old_directory_len;
	long long file_len = 0;

	//ABRIMOS EL DIRECTORIO
	if((directory_stream = opendir(argv[1])) == NULL)
	{
		printf("Error: cannot open the directory.\n");
		exit(EXIT_FAILURE);
	}

	//CAMBIAMOS DE DIRECTORIO
	if((SetCurrentDirectory(argv[1])) == 0)
	{
		printf("Error: cannot change the directory.\n");
		exit(EXIT_FAILURE);
	}

	//MOSTRAMOS EL NOMBRE DE LOS ARCHIVOS DENTRO DEL DIRECTORIO
	printf("Files in the directory: \n");
	while((file_pointer = readdir(directory_stream)) != NULL)
		printf(">> %s\n", file_pointer->d_name);

	for(;;)
	{
		printf("\n\nMENU\n");
		printf("1. Show contents of some file.\n");
		printf("2. Copy one or more files to another directory (it musts previously exist)\n");
		printf("3. Exit\n");
		printf("Choose an option: ");
		scanf("%hd", &option);
		switch(option)
		{
			case 1:
				printf("Write the name of the file: ");
				scanf("%s", file_name);
				if((file_descriptor = CreateFile(file_name, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE)
				{
					printf("Error: cannot open the file.\n");
					exit(EXIT_FAILURE);
				}
				printf("Contents: \n");
				file_buf = malloc(2);
				if((ReadFile(file_descriptor, file_buf, 98, NULL, NULL)) == 0)
				{
					printf("Error: failed to read the file\n");
					exit(EXIT_FAILURE);
				}
				printf(" %s\n", file_buf);
				CloseHandle(file_descriptor);
			break;
			case 2:
				printf("Enter the directory you want to paste the files: ");
				scanf("%s", new_directory);
				printf("Enter the number of files you want to copy: ");
				scanf("%hd", &no_cfiles);
				file_cnames = malloc(no_cfiles * sizeof(char *)); //asignando espacio para el número de nombres de archivos a guardar
				//PEDIMOS EL NOMBRE DE CADA ARCHIVO Y LOS CREAMOS EN EL NUEVO DIRECTORIO
				for(i = 0; i < no_cfiles; i++)
				{	
					//ASIGNANDO ESPACIO PARA EL NOMBRE DE CADA ARCHIVO
					file_cnames[i] = malloc(FILENAME_MAX);	
					printf("Enter the name of the %hd file: ", i + 1);
					scanf("%s", file_cnames[i]);

					//CAMBIAMOS AL VIEJO DIRECTORIO
					if((SetCurrentDirectory(argv[1])) == 0)
					{
						printf("Error: failed to change to old directory.\n");
						exit(EXIT_FAILURE);
					}

					//ABRIMOS EL VIEJO ARCHIVO
					if((file_descriptor = CreateFile(file_cnames[i], GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE)
					{
						printf("Error: failed to open %s\n", file_cnames[i]);
						exit(EXIT_FAILURE);
					}

					/*
					//CALCULAMOS EL NÚMERO DE CARACTERES EN EL ARCHIVO VIEJO
					while((read(file_descriptor, file_buf, 1)) != 0)
						file_len++;

					//NOS COLOCAMOS DE NUEVO EN EL INCIO DEL ARCHIVO
					lseek(file_descriptor, 0, SEEK_SET);
					*/

					//ASIGNAMOS ESPACIO EN EL BUFFER DE ACUERDO AL TAMAÑO DEL ARCHIVO
					file_buf = malloc(100);

					//LEEMOS TODO EL ARCHIVO Y LO GUARDAMOS EN FILE_BUF
					if((ReadFile(file_descriptor, file_buf, 90, NULL, NULL)) == 0)
					{
						printf("Error: failed to read the file\n");
						exit(EXIT_FAILURE);
					}

					//CERRAMOS EL VIEJO ARCHIVO
					CloseHandle(file_descriptor);

					//CAMBIAMOS AL NUEVO DIRECTORIO
					if((SetCurrentDirectory(new_directory)) == 0)
					{
						printf("Error: failed to change to new directory.\n");
						exit(EXIT_FAILURE);
					}

					//CREANDO EL ARCHIVO CON EL NOMBRE ESPECIFICADO
					if((file_descriptor = CreateFile(file_cnames[i], GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE)
					{
						printf("Error: failed to creat the file %s\n", file_cnames[i]);
						exit(EXIT_FAILURE);
					}

					//ESCRIBIMOS HACIA EL NUEVO ARCHIVO EL CONTENIDO DEL VIEJO
					WriteFile(file_descriptor, file_buf, 90, NULL, NULL);

					//CERRAMOS EL NUEVO ARCHIVO
					CloseHandle(file_descriptor);
				}
			break;
			case 3:
				exit(EXIT_SUCCESS);
			break;
			default:
				printf("Choose a correct option.\n");
			break;
		}
	}
	
	printf("Choose \n");
	return 0;
}