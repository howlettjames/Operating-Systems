#include <windows.h>
#include <stdio.h>

int main(void)
{
	char mensaje[20];
	DWORD leidos;
	HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);
	SECURITY_ATTRIBUTES pipeSeg = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE};
		
	//Lectura desde la tubería sin nombre
	ReadFile(hStdIn, mensaje, sizeof(mensaje), &leidos, NULL);
	printf("Mensaje recibido del proceso padre: %s\n", mensaje);
	CloseHandle(hStdIn);
	printf("Termina el proceso hijo, continua el proceso padre\n");

	return 0;
}