#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define TAM_MEM 27

int main(void)
{
	HANDLE hArchMapeo;
	char *idMemCompartida = "MemoriaCompartida";
	char *apDatos, *apTrabajo, c;

	if((hArchMapeo = OpenFileMapping(
		FILE_MAP_ALL_ACCESS, //Acceso lectura/escritura de la memoria compartida
		FALSE, //No se hereda el nombre
		idMemCompartida //Identificador de la memoria compartida
		)) == NULL)
	{
		printf("No se abrio archivo de mapeo la memoria compartida: (%i)\n", GetLastError());
		exit(EXIT_FAILURE);
	}

	if((apDatos = (char *)MapViewOfFile(
		hArchMapeo, //Manejador del mapeo
		FILE_MAP_ALL_ACCESS, //Permiso de lectura/escritura en la memoria
		0,
		0,
		TAM_MEM
		)) == NULL)
	{
		printf("No se accedio a la memoria compartida: (%i)\n", GetLastError());
		CloseHandle(hArchMapeo);
		exit(EXIT_FAILURE); 
	}

	for(apTrabajo = apDatos; *apTrabajo != '\0'; apTrabajo++)
		putchar(*apTrabajo);
	putchar('\n');
	*apDatos = '*';

	UnmapViewOfFile(apDatos);
	CloseHandle(hArchMapeo);

	return 0;
}