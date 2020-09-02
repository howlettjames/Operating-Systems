#include <stdio.h>
#include <windows.h>

int main(int argc, char *argv[])
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	HANDLE hSemaforo;
	int i = 1;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	if(argc != 2)
	{
		printf("Usar: %s Nombre_programa_hijo\n", argv[0]);
		return -1;
	}

	//Creacion del semáforo
	if((hSemaforo = CreateSemaphore(NULL, 1, 1, "Semaforo")) == NULL)
	{
		printf("Falla al invocar CreateSemaphore: %d\n", GetLastError());
		return -1;
	}

	//Creación proceso hijo
	if(!CreateProcess(NULL, argv[1], NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
	{
		printf("Falla al invocar CreateProcess: %d\n", GetLastError());
		return -1;
	}

	while(i < 4)
	{
		//Prueba del semáforo
		WaitForSingleObject(hSemaforo, INFINITE);

		//Sección crítica
		printf("Soy el padre entrando %i de 3 veces al semaforo\n", i);
		Sleep(5000);

		//Liberación del semáforo
		if(!ReleaseSemaphore(hSemaforo, 1, NULL))
		{
			printf("Falla al invocar ReleaseSemaphore: %d\n", GetLastError());
		}
		printf("Soy el padre liberando %i de 3 veces al semaforo\n", i);
		Sleep(5000);

		i++;
	}

	//Terminación controlada del proceso e hilo asociado de ejecución
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}