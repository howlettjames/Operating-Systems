#include <stdio.h>
#include <windows.h>

int main(int argc, char *argv[])
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	DWORD pid = GetCurrentProcessId();

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	if(argc != 2)
	{
		printf("Usar: %s Nombre_programa_hijo\n", argv[0]);
		return 0;
	}

	//Creación proceso hijo
	if(!CreateProcess(NULL, argv[1], NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
	{
		printf("Error: failed to invoke CreateProcess(%d)\n", GetLastError());
		return 0;
	}

	//Proceso padre
	printf("I'm the root | ID: %d\n", pid);
	WaitForSingleObject(pi.hProcess, INFINITE);
	
	//Terminación controlada del proceso e hilo asociado de ejecución
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}