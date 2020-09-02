#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h> 
#include <string.h>

//PARA PODER CAMBIAR EL NUMERO DE CADENAS, LA LONGITUD DE CADA UNA DE ELLAS, Y EL NO DE ARCHIVOS
#define NO_STRINGS 10
#define LENGTH_STRINGS 100
#define NO_FILES 5

int main(int argc, char const *argv[])
{
	int file_d[NO_FILES];
	short i, j;
	char *strings[NO_STRINGS];
	char *files[NO_FILES];
	char filea[6] = "a.txt";
	char fileb[6] = "b.txt";
	char filec[6] = "c.txt";
	char filed[6] = "d.txt";
	char filee[6] = "e.txt";

	srand((unsigned) time(NULL));

	//CREANDO LAS CADENAS CON LETRAS ALEATORIAS A Y B
	for(i = 0; i < NO_STRINGS; i++)
	{
		strings[i] = malloc(LENGTH_STRINGS);
		for(j = 0; j < LENGTH_STRINGS - 1; j++)
		{
			if(rand() % 2)
				strings[i][j] = 'a';
			else
				strings[i][j] = 'b';
		}
		strings[i][j] = '\0';
	}

	//ASIGNANDO LOS NOMBRES DEL LOS ARCHIVOS AL ARREGLO DE APUNTADORES A CADENAS PARA POSTERIOR PROCESO
	files[0] = filea;
	files[1] = fileb;
	files[2] = filec;
	files[3] = filed;
	files[4] = filee;

	//CREANDO EL DIRECTORIO CON LA RUTA ESPECIFICADA DESDE CONSOLA
	if((mkdir(argv[1], 0777)) == -1)
	{
		printf("Error: Cannot create the directory.\n");
		exit(EXIT_FAILURE);
	}

	//CAMBIANDO DE DIRECTORIO
	if((chdir(argv[1])) == -1)
	{
		printf("Error: Cannot change the directory.\n");
		exit(EXIT_FAILURE);	
	}
	
	//CREANDO LOS ARCHIVOS DENTRO DEL DIRECTORIO Y ESCRIBIENDO EN ELLOS LAS CADENAS CREADAS ANTERIORMENTE
	for(i = 0; i < NO_FILES; i++)
	{
		if((file_d[i] = creat(files[i], S_IRWXU)) == -1)
		{
			printf("Error: Cannot create the file.\n");
			exit(EXIT_FAILURE);	
		}
		write(file_d[i], strings[rand() % NO_STRINGS], LENGTH_STRINGS - 1);
		close(file_d[i]);
	}

	return 0;
}