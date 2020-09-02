#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <Windows.h>

//PARA PODER CAMBIAR EL NUMERO DE CADENAS, LA LONGITUD DE CADA UNA DE ELLAS, Y EL NO DE ARCHIVOS
#define NO_STRINGS 10
#define LENGTH_STRINGS 100
#define NO_FILES 5

int main(int argc, char const *argv[])
{
	HANDLE file_d[NO_FILES];
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
	if((CreateDirectory(argv[1], NULL)) == 0)
	{
		printf("Error: Cannot create the directory.\n");
		exit(EXIT_FAILURE);
	}

	//CAMBIANDO DE DIRECTORIO
	if((SetCurrentDirectory(argv[1])) == 0)
	{
		printf("Error: Cannot change the directory.\n");
		exit(EXIT_FAILURE);	
	}
	
	//CREANDO LOS ARCHIVOS DENTRO DEL DIRECTORIO Y ESCRIBIENDO EN ELLOS LAS CADENAS CREADAS ANTERIORMENTE
	for(i = 0; i < NO_FILES; i++)
	{
		if((file_d[i] = CreateFile(files[i], GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE)
		{
			printf("Error: Cannot create the file.\n");
			exit(EXIT_FAILURE);	
		}
		WriteFile(file_d[i], strings[rand() % NO_STRINGS], LENGTH_STRINGS - 1, NULL, NULL);
		CloseHandle(file_d[i]);
	}

	return 0;
}