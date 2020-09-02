#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>

//EL NO DE ARCHIVOS QUE SE SUPONE HAY COMO MÁXIMO EN UN DIRECTORIO
#define NO_FILES 10

int main(int argc, char const *argv[])
{
	DIR *directory_stream;
	struct dirent *filep;
	struct stat file_status[NO_FILES];
	char *file_names[NO_FILES];
	short i = 0, j;

	//ABRIMOS EL DIRECTORIO
	if((directory_stream = opendir(argv[1])) == NULL)
	{
		printf("Error: cannot open the directory.\n");	
		exit(EXIT_FAILURE);
	}

	//CAMBIAMOS DE DIRECTORIO PARA PODER USAR NOMBRE RELATIVOS
	if((chdir(argv[1])) == -1)
	{
		printf("Error: cannot change the directory.\n");
		exit(EXIT_FAILURE);
	}

	//GUARDAMOS LOS ESTATUS DE LOS ARCHIVOS DENTRO DEL DIRECTORIO EN UN ARREGLO 
	while((filep = readdir(directory_stream)) != NULL)
	{
		if((stat(filep->d_name, &file_status[i])) == -1)
		{
			printf("Error: cannot read file status of file: %s\n", filep->d_name);
			exit(EXIT_FAILURE);
		}
		file_names[i++] = filep->d_name;
	}

	//IMPRIMIMOS LA INFORMACIÓN DE CADA ARCHIVO
	for(j = 0; j < i; j++)
		printf(">> Name: %s  Size: %lld bytes  Last file access: %s", file_names[j], (long long) file_status[j].st_size, ctime(&file_status[j].st_atime));

	closedir(directory_stream);

	return 0;
}
















