#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define TAM_MEM 27

int main(void)
{
	HANDLE hArchMapeo;
	char *idMemCompartida = "MemoriaCompartida";
	char *apDatos, *apTrabajo, c;

	if((hArchMapeo = CreateFileMapping(
		INVALID_HANDLE_VALUE, //Usa memoria compartida
		NULL, //Seguridad por default
		PAGE_READWRITE, //Acceso escritura/lectura a la memoria
		0, //Tamaño maximo parte alta de un DWORD
		TAM_MEM, //Tamaño maximo parte baja de un DWORD
		idMemCompartida //Identificador de la memoria compartida
		)) == NULL)
	{
		printf("No se mapeo la memoria compartida: (%i)\n", GetLastError());
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
		printf("No se creo la memoria compartida: (%i)\n", GetLastError());
		CloseHandle(hArchMapeo);
		exit(EXIT_FAILURE); 
	}

	apTrabajo = apDatos;
	for(c = 'a'; c <= 'z'; c++)
		*apTrabajo++ = c;
	*apTrabajo = '\0';
	while(*apDatos != '*')
		Sleep(1);

	UnmapViewOfFile(apDatos);
	CloseHandle(hArchMapeo);

	return 0;
}